#!/usr/bin/env python3

import os
import argparse
import numpy   as np

from   pathlib import Path
from   PIL     import Image

import torch
import torch.nn.functional as F

def load_paths(directory, t = ".png"):
    paths = []
    for root, dirs, files in os.walk(directory):
        # os.walk returns files in arbitrary order
        for f in sorted(files):
            if f.endswith(t):
                paths.append(os.path.join(root, f))
    return paths

def process_fn(image_path):
    if t == ".npy":
        return torch.Tensor(np.load(image_path).astype(np.uint8))
    else:
        return torch.Tensor(np.array(Image.open(image_path).convert('RGB'), dtype=np.uint8))

def save_losses(losses, loss_dest):
    with open(loss_dest, "w") as f:
        f.writelines([f"{loss}\n" for loss in losses])

if __name__ == "__main__":
    # Command-line setup
    parser = argparse.ArgumentParser(
        prog="Dataset distance calculator",
        description="Calculates the L2 distance between a paired dataset, often one before and one after a processing step"
    )
    parser.add_argument("--a", required=True)
    parser.add_argument("--b", required=True)
    parser.add_argument("--result", required=True)
    parser.add_argument("--name", required=True)

    args = parser.parse_args()

    ds_a     = Path(args.a)
    ds_b     = Path(args.b)
    res_file = Path(args.output) / f"{args.name}.txt"

    print(ds_b, ds_a, res_dir)

    ds_a_paths = findAllFiles(ds_a)
    ds_b_paths = findAllFiles(ds_b)


    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    losses = [F.mse_loss(a, b) for a, b in zip(ds_a_paths, ds_b_paths)]

    save_losses(losses, res_file)
