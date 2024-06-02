#!/usr/bin/env python3
import os
import argparse

# Files
import dng
import cfa

# Libs
import numpy     as     np
from   pathlib   import Path
from   PIL       import Image
from   itertools import product


def findAllFiles(path):
    res = []
    path, folders, files = next(os.walk(path))

    for f in folders:
        res.extend(findAllFiles(os.path.join(path, f)))

    res.extend([os.path.join(path, f) for f in files
                if os.path.splitext(f)[1].lower() in ['.png', '.tiff', '.jpeg', '.jpg']])
    return res


def convert_ds_category(ds_path, res_dir, suffix, size):
    save_path_png = res_dir / "png" / suffix
    save_path_dng = res_dir / "dng" / suffix
    save_path_npy = res_dir / "npy" / suffix
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

        # Save RGB image
        Image.fromarray(img_crop).save(save_path_png / f"{n}.png", format="PNG")

        # Save CFA
        img_cfa = cfa.rgb_to_cfa(img_crop, cfa.rgb_kf)

        dng.write(img_cfa.astype(np.uint16) << 8, filename=str(save_path_dng / str(n)))
        np.save(save_path_npy / str(n), img_cfa)


if __name__ == "__main__":
    # Command-line setup
    parser = argparse.ArgumentParser(
        prog="dataset CFA converter",
        description="Converts a dataset to CFA, outputs in .npy (CFA), .dng (CFA), and .png (Unprocessed RGB)."
    )
    parser.add_argument("--input",  required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--size",   required=True)

    args = parser.parse_args()

    ds_path     = Path(args.input)
    ds_cats     = [entry.name for entry in os.scandir(ds_path) if entry.is_dir()]
    res_dir     = Path(args.output)

    print(ds_path, ds_cats, res_dir)

    os.makedirs(res_dir, exist_ok=True)

    # Process
    for suffix in ds_cats:
        convert_ds_category(ds_path, res_dir, suffix, int(args.size))
