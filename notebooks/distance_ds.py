#!/usr/bin/env python3

import os
import argparse
import numpy   as np

from   pathlib import Path
from   PIL     import Image

import torch
import torch.nn.functional as F

def load_paths(directory):
    paths = []
    for root, dirs, files in os.walk(directory):
        # os.walk returns files in arbitrary order
        for f in sorted(files):
            if os.path.splitext(f)[1].lower() in ['.png', '.tiff', '.jpeg', '.jpg']:
                paths.append(os.path.join(root, f))
    return paths

def process_fn(image_path):
    _, t = os.path.splitext(image_path)
    if t == ".npy":
        return torch.Tensor(np.load(image_path).astype(np.float32))
    else:
        return torch.Tensor(np.array(Image.open(image_path).convert('RGB'), dtype=np.float32))

def save_losses(losses, loss_dest):
    with open(loss_dest, "w") as f:
        f.writelines([f"{loss}\n" for loss in losses])

if __name__ == "__main__":
    # Command-line setup
    parser = argparse.ArgumentParser(
        prog="Dataset distance calculator",
        description="Calculates the L2 distance between a paired dataset, often one before and one after a processing step"
    )
    parser.add_argument("--clean",  required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--name",   required=True)
    parser.add_argument("--path",   required=True)
    parser.add_argument("subfolders", nargs="+", help="Folders to calculate distance against")

    args = parser.parse_args()

    ds_a       = Path(args.clean)
    ds_a_paths = load_paths(ds_a)

    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    print(args)

    for folder in args.subfolders:
        res_file = Path(args.output) / f"{folder}-det-loss.txt"

        ds_b       = Path(args.path) / folder
        ds_b_paths = load_paths(ds_b)

        losses = [F.mse_loss(process_fn(a) / 256.0,
                             process_fn(b) / 256.0)
                  for a, b in zip(ds_a_paths, ds_b_paths)]

        save_losses(losses, res_file)
