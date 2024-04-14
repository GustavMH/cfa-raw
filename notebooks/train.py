#!/usr/bin/env python3

import os
import joblib
import pickle
import argparse

from pathlib import Path

import numpy             as np
import matplotlib.pyplot as plt
from   PIL import Image


import torch
import torch.nn.functional as F
from   torch import nn, optim
from   torch.utils.data import DataLoader, TensorDataset, Dataset, random_split, RandomSampler

from denoisingautoencoder import DenoisingAutoencoder
from cfa                  import colorize_cfa, rgb_kf

def load_images(directory, t = ".png"):
    """
    Load images from dir

    Parameters:
    - directory (str): directory to get data
    - target_size (tuple): expected size of images without channels

    Returns:
    - data (torch.Tensor): A torch stack with the data
    """
    images = []
    for root, dirs, files in os.walk(directory):
        # os.walk returns files in arbitrary order
        for file in sorted(files):
            if file.endswith(t):
                image_path = os.path.join(root, file)
                image_tensor = (
                    torch.Tensor(np.array(Image.open(image_path).convert('RGB'), dtype=np.float32)).permute(2,0,1)
                    if t == ".tiff" or t == ".png"
                    else torch.Tensor(colorize_cfa(np.load(image_path), rgb_kf).astype(np.float32)).permute(2,0,1)
                )

                if image_tensor.max() > 1:
                    image_tensor /= 256.0

                images.append(image_tensor)
    return torch.stack(images)

class PairedDataset(Dataset):
    def __init__(self, data_clean, data_noisy):
        self.data_clean = data_clean
        self.data_noisy = data_noisy

    def __len__(self):
        # Assuming both datasets have the same length
        return len(self.data_clean)

    def __getitem__(self, idx):
        clean_image = self.data_clean[idx]
        noisy_image = self.data_noisy[idx]

        return clean_image, noisy_image

def train(train_clean, train_noise, n_epochs=50):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    paired_dataset = PairedDataset(train_clean, train_noise)
    paired_loader  = DataLoader(paired_dataset, batch_size=32, shuffle=True)

    model = DenoisingAutoencoder().to(device)
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=1e-4)

    for epoch in range(n_epochs):
        model.train()
        running_loss = 0.0

        for clean_images, noisy_images in paired_loader:
            clean_images, noisy_images = clean_images.to(device), noisy_images.to(device)

            optimizer.zero_grad()

            #with torch.autocast(device_type=device, dtype=torch.float16):
            # Forward pass
            outputs = model(noisy_images)
            loss = criterion(outputs, clean_images)

            loss.backward()
            optimizer.step()

            running_loss += loss.item()

        epoch_loss = running_loss / len(paired_loader)
        print(f'Epoch {epoch+1}/{n_epochs}, Loss: {epoch_loss}')

    for param in model.parameters():
        if torch.isnan(param).any() or torch.isinf(param).any():
            print("Model contains NaN or inf values")

    return model


def save_model(model, model_dest):
    joblib.dump(model, Path(model_dest))

def save_losses(losses, loss_dest):
    with open(loss_dest, "w") as f:
        f.writelines([f"{loss}\n" for loss in losses])

def validation_imgs(model, inputs_clean, inputs_noise, savepath=None):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    with torch.no_grad():
        outputs = model(inputs_noise[:3].to(device)).cpu()

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

    args = parser.parse_args()

    print("DEVICE:", torch.device("cuda:0" if torch.cuda.is_available() else "cpu"))
    print(args)

    if not (args.epochs or args.model):
        print("Nothing to do!")

    if args.epochs:
        train_clean = load_images(Path(args.clean) / "train", t=".png")
        train_noise = load_images(Path(args.noise) / "train", t=args.type)

        model = train(train_clean, train_noise, n_epochs=int(args.epochs))
        save_model(model, model_dest=Path(args.output) / f"{args.name}-model.pkl")

    if args.model:
        model = pickle.load(open(Path(args.model), "rb"))

        val_clean = load_images(Path(args.clean) / "val", t=".png")
        val_noise = load_images(Path(args.noise) / "val", t=args.type)

        losses = validate(model, val_clean, val_noise)
        save_losses(losses, loss_dest=Path(args.output) / f"{args.name}-val-loss.txt")

        validation_imgs(model, val_clean, val_noise, savepath=Path(args.output) / f"{args.name}-imgs.png")
