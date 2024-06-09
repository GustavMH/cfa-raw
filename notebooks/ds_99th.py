#!/usr/bin/env python3

import os
import argparse
import numpy   as np

from   pathlib import Path
from   PIL     import Image

import torch
import torch.nn.functional as F

import resource
def using(point=""):
    usage=resource.getrusage(resource.RUSAGE_SELF)
    print( '''%s: usertime=%s systime=%s mem=%s mb
           '''%(point,usage[0],usage[1],
                usage[2]/1024.0 ), flush=True)

def load_paths(directory):
    paths = []
    for root, dirs, files in os.walk(directory):
        # os.walk returns files in arbitrary order
        for f in sorted(files):
            if os.path.splitext(f)[1].lower() in ['.png', '.tiff', '.jpeg', '.jpg']:
                paths.append(os.path.join(root, f))
    return paths

def process_fn(image_path):
    return torch.Tensor(np.array(Image.open(image_path).convert('RGB'), dtype=np.float16))

def save_losses(losses, loss_dest):
    with open(loss_dest, "w") as f:
        f.writelines([f"{loss}\n" for loss in losses])

if __name__ == "__main__":
    # Command-line setup
    parser = argparse.ArgumentParser(
        prog="Dataset quantile calculator",
        description="Calculates the 1st and 99th quantile in a dataset"
    )
    parser.add_argument("--output", required=True)
    parser.add_argument("--name",   required=True)
    parser.add_argument("--path",   required=True)
    parser.add_argument("subfolders", nargs="+", help="Folders to calculate distance against")

    args = parser.parse_args()

    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    print(args, flush=True)

    for folder in args.subfolders:
        using(folder)

        ds_a       = Path(args.path) / folder
        ds_a_paths = load_paths(ds_a)

        nnth = []
        onth = []
        for a in ds_a_paths:
            img = process_fn(a).to(device)
            nnth.append(torch.quantile(img / 256.0, 0.99).detach().cpu().item())
            onth.append(torch.quantile(img / 256.0, 0.01).detach().cpu().item())

        save_losses(nnth, Path(args.output) / f"{args.name}-{folder}-det-99.txt")
        save_losses(onth, Path(args.output) / f"{args.name}-{folder}-det-1.txt")
