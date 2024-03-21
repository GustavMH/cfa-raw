#!/usr/bin/env python3

import os
import argparse

# Files
import dng
import cfa
import noise

# Libs
import numpy           as     np
from   pathlib         import Path
from   PIL             import Image
from   itertools       import product
from   multiprocessing import Pool, cpu_count


def findAllFiles(path):
    res = []
    path, folders, files = next(os.walk(path))
    if folders:
        for f in folders:
            res.extend(findAllFiles(os.path.join(path, f)))
    res.extend([os.path.join(path, f) for f in files if not f.endswith(".txt")])
    return res


def add_noise_to_ds_category(ds_path, res_dir, suffix, noise, noise_amt, size):
    noise_type, noise_func = noise

    save_path_png = res_dir / "png" / noise_type / f"{noise_amt*100:g}pct" / suffix
    save_path_dng = res_dir / "dng" / noise_type / f"{noise_amt*100:g}pct" / suffix
    save_path_npy = res_dir / "npy" / noise_type / f"{noise_amt*100:g}pct" / suffix
    os.makedirs(save_path_png, exist_ok=True)
    os.makedirs(save_path_dng, exist_ok=True)
    os.makedirs(save_path_npy, exist_ok=True)

    imgs = findAllFiles(ds_path / suffix)

    for n, img in enumerate(imgs):
        img_rgb = np.array(Image.open(img).convert("RGB"), dtype=np.uint8)

        # Center crop
        h, w, c  = img_rgb.shape
        offset_x = (h - size) // 2
        offset_y = (w - size) // 2

        img_crop      = np.zeros((size,size,c))
        img_crop      = img_rgb[offset_x:(offset_x+size), offset_y:(offset_y+size)]
        img_rgb_noise = noise_func(img_crop, amount=noise_amt)

        # Save Noisy RGB image
        Image.fromarray(img_rgb_noise).save(save_path_png / f"{n}.png", format="PNG")

        img_cfa = cfa.rgb_to_cfa(img_rgb_noise.astype(np.uint16) * 2**8, cfa.rgb_kf)

        # Save noisy CFA
        dng.write(img_cfa, filename=str(save_path_dng / str(n)))
        np.save(save_path_npy / str(n), img_cfa)

def _f(args):
    add_noise_to_ds_category(*args)

def identity(x, **kvargs):
    return x

def p_add_noise_to_ds(ds_path, res_dir, suffixes, noises, noise_amts, size):
    args_noise = [[ds_path, res_dir, *args, size]
                  for args in product(suffixes, noises, noise_amts)]
    args_clean = [[ds_path, res_dir, suffix, ("none", identity), 0, size]
                  for suffix in suffixes]

    p = Pool(processes=cpu_count())
    p.map(_f, args_noise + args_clean)


noises = [
    ("gaussian",   noise.gaussian_noise),
    ("speckle",    noise.norm_speckle_noise),
    ("saltpepper", noise.salt_pepper_noise)
]


if __name__ == "__main__":
    # Command-line setup
    parser = argparse.ArgumentParser(
        prog="dataset noise augmenter and CFA converter",
        description="Adds Gaussian, Salt-and-Pepper, and Speckle noise to a dataset, and output the augmented data in .npy (CFA), .dng (CFA), and .png (RGB)."
    )
    parser.add_argument("--input", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--size", required=True)
    parser.add_argument("noise", nargs="+", help="Amount of noise (%) to augment with")

    args = parser.parse_args()

    ds_path     = Path(args.input)
    ds_cats     = [entry.name for entry in os.scandir(ds_path) if entry.is_dir()]
    ds_img_size = int(args.size)
    res_dir     = Path(args.output)
    noise_amts  = [int(n) / 100 for n in args.noise]

    print(
        ds_path,
        ds_cats,
        ds_img_size,
        res_dir,
        noise_amts
    )
    os.makedirs(res_dir, exist_ok=True)

    # Process
    p_add_noise_to_ds(ds_path, res_dir, ds_cats, noises, noise_amts, ds_img_size)
