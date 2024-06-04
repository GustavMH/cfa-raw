#!/usr/bin/env python3

import os
import joblib
import pickle
import argparse

from pathlib import Path

import numpy             as np
import matplotlib.pyplot as plt
from   PIL    import Image
from   random import randint

import torch
import torch.nn.functional as F
from   torch            import nn, optim
from   torch.utils.data import DataLoader, TensorDataset, Dataset, random_split, RandomSampler
from   torch.cuda.amp   import GradScaler

from denoisingautoencoder import DenoisingAutoencoder
from cfa                  import colorize_cfa, rgb_kf

import gc

# Memory benchmark
import tracemalloc
import resource
import sys
def using(point=""):
    usage=resource.getrusage(resource.RUSAGE_SELF)
    print( '''%s: usertime=%s systime=%s mem=%s mb
           '''%(point,usage[0],usage[1],
                usage[2]/1024.0 ), flush=True)

def expand_cfa(tensor, dims=3):
    return torch.stack([torch.Tensor(tensor)] * dims)

def load_images(directory, t = ".png", expand_cfa_p = False, crop=False):
    """
    Load images from dir

    Parameters:
    - directory (str): directory to get data
    - target_size (tuple): expected size of images without channels

    Returns:
    - data (torch.Tensor): A torch stack with the data
    """
    paths = []
    for root, dirs, files in os.walk(directory):
        # os.walk returns files in arbitrary order
        for f in sorted(files):
            if f.endswith(t):
                paths.append(os.path.join(root, f))

    def process_fn(image_path):
        if t == ".npy":
            if expand_cfa_p:
                return torch.Tensor(expand_cfa(np.load(image_path).astype(np.uint8)))
            else:
                return torch.Tensor((colorize_cfa(np.load(image_path), rgb_kf)).astype(np.uint8)).permute(2,0,1)
        else:
             return torch.Tensor(np.array(Image.open(image_path).convert('RGB'), dtype=np.uint8)).permute(2,0,1)

    first  = process_fn(paths[0])
    res    = torch.zeros([len(paths), *first.shape], dtype=torch.uint8)
    res[0] = first

    for i, path in enumerate(paths[1:]):
        res[i] = process_fn(path)

    return res

def cfaAugment(img, rot):
    x, y = [(0,0), (0,1), (1,0), (1,1)][rot]
    c, w, h = img.shape

    # The denoiser has to be aligned to 32x32 px chunks
    return img[:, x:w+x-32, y:h+y-32]

class PairedDataset(Dataset):
    def __init__(self, data_clean, data_noisy, cfa_aug=False, cfa_rand_scale=False):
        self.cfa_aug = cfa_aug
        self.cfa_rand_scale = cfa_rand_scale
        self.data_clean = data_clean
        self.data_noisy = data_noisy

    def __len__(self):
        # Assuming both datasets have the same length
        return len(self.data_clean)

    def __getitem__(self, idx):
        clean_image = self.data_clean[idx]
        noisy_image = self.data_noisy[idx]

        if self.cfa_aug:
            r = randint(0,3)

            clean_image = cfaAugment(clean_image, r)
            noisy_image = cfaAugment(noisy_image, r)

        if self.cfa_rand_scale:
            r, g, b = self.cfa_rand_scale[idx]
            noisy_image = noisy_image.to(torch.float32)
            noisy_image[0] *= r
            noisy_image[1] *= g
            noisy_image[2] *= b

        # The data is stored as uint8
        # and returned as normalized float32
        return (
            clean_image.to(torch.float32) / 256.0,
            noisy_image.to(torch.float32) / 256.0
        )

def train(paired_dataset, n_epochs=50, loss=None):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    paired_loader  = DataLoader(paired_dataset, batch_size=32, shuffle=True)

    model = DenoisingAutoencoder().to(device)

    criterion = loss
    optimizer = optim.AdamW(model.parameters(), lr=1e-4)

    scaler = GradScaler()

    for epoch in range(n_epochs):
        model.train()
        running_loss = 0.0

        for clean_images, noisy_images in paired_loader:
            clean_images, noisy_images = clean_images.to(device), noisy_images.to(device)


            optimizer.zero_grad()

            with torch.autocast(device_type="cuda", dtype=torch.float16):
                # Forward pass
                outputs = model(noisy_images)
                loss = criterion(outputs, clean_images)

            scaler.scale(loss).backward()
            scaler.step(optimizer)

            scaler.update()

            running_loss += loss.item()

        epoch_loss = running_loss / len(paired_loader)
        print(f'Epoch {epoch+1}/{n_epochs}, Loss: {epoch_loss}', flush=True)

    for param in model.parameters():
        if torch.isnan(param).any() or torch.isinf(param).any():
            print("Model contains NaN or inf values", flush=True)

    return model


def save_model(model, model_dest):
    joblib.dump(model, Path(model_dest))

def save_losses(losses, loss_dest):
    with open(loss_dest, "w") as f:
        f.writelines([f"{loss}\n" for loss in losses])

def validation_imgs(model, inputs_clean, inputs_noise, savepath=None):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    with torch.no_grad():
        outputs = model((inputs_noise[:3].to(torch.float32) / 256).to(device)).cpu()

        fig, axs = plt.subplots(3, 3, figsize=(15, 6))
        for i in range(3):
            for n, (title, data) in enumerate([
                ("Target image", inputs_clean[:3]),
                ("Input noisy",  inputs_noise[:3]),
                ("Result image", outputs)
            ]):
                axs[n, i].imshow(data[i].permute(1, 2, 0).squeeze())
                axs[n, i].title.set_text(title + str(i+1))
                axs[n, i].axis('off')

        if savepath:
            plt.savefig(savepath)
        else:
            plt.show()


def validate(model, val_clean, val_noise):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    paired_dataset = DataLoader(PairedDataset(val_clean, val_noise), batch_size=128, shuffle=False)

    model.eval()
    model.to(device)

    def loss(inputs_clean, inputs_noise):
        outputs = model(inputs_noise.to(device))
        mse = F.mse_loss(outputs, inputs_clean.to(device))
        return mse.cpu()

    with torch.no_grad():
        return [loss(*val) for val in paired_dataset]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="train.py",
        description="Trains a denoising model"
    )
    parser.add_argument("--clean", required=True, help="Path to the clean data")
    parser.add_argument("--noise", required=True, help="Path to the noisy data")
    parser.add_argument("--output", required=True, help="Path to the output folder, model and loss are saved here")
    parser.add_argument("--name", required=True, help="Name prefix for the output files")
    parser.add_argument("--type", required=True, help="Noise input file type")
    parser.add_argument("--epochs")
    parser.add_argument("--model")
    parser.add_argument("--loss", help="Loss function to use during training", default="L2", choices=["L1", "L1smooth", "L2"])
    parser.add_argument("--cfa-augment", action="store_true")
    parser.add_argument("--cfa-expand", action="store_true")
    parser.add_argument("--cfa-scale-file")

    args = parser.parse_args()

    print("DEVICE:", torch.device("cuda:0" if torch.cuda.is_available() else "cpu"), flush=True)
    print(args, flush=True)

    val_noise = load_images(Path(args.noise) / "val", t=args.type, expand_cfa_p=args.cfa_expand)


    if not torch.cuda.is_available():
        print("No cuda device")
        exit(1)

    if not (args.epochs or args.model):
        print("Nothing to do!", flush=True)

    if args.epochs:
        using("Start")
        train_clean = load_images(Path(args.clean) / "train", t=".png")
        using("Clean data loaded")
        train_noise = load_images(Path(args.noise) / "train", t=args.type, expand_cfa_p=args.cfa_expand)
        using("Noisy data loaded")
        print((train_noise.numel() * train_noise.element_size()) / 10**6, "MB", train_noise.shape, train_noise.dtype)

        loss = {
            "L2": nn.MSELoss(),
            "L1": nn.L1Loss(),
            "L1smooth": nn.SmoothL1Loss()
        }[args.loss]

        scale_list = None
        if args.cfa_scale_file:
            scale_list = torch.Tensor(np.load(f))

        paired_dataset = PairedDataset(train_clean, train_noise, args.cfa_augment, scale_list)
        using("Dataset created")
        model = train(paired_dataset, n_epochs=int(args.epochs), loss=loss)
        save_model(model, model_dest=Path(args.output) / f"{args.name}-model.pkl")

    if args.model:
        model = pickle.load(open(Path(args.model), "rb"))

        val_clean = load_images(Path(args.clean) / "val", t=".png")
        val_noise = load_images(Path(args.noise) / "val", t=args.type, expand_cfa_p=args.cfa_expand)

        losses = validate(model, val_clean, val_noise)
        save_losses(losses, loss_dest=Path(args.output) / f"{args.name}-val-loss.txt")

        validation_imgs(model, val_clean, val_noise, savepath=Path(args.output) / f"{args.name}-imgs.png")
