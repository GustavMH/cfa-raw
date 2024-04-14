#!/usr/bin/env python3
from itertools import product
from pathlib   import Path

forms = [("npy","npy"), ("ahd","tiff"), ("png","png")]
noise_types = ["gaussian", "speckle", "saltpepper"]
intensities = [
    '64', '32', '16', '8',
    '4', '2', '1', '0.5',
    '0.25', '0.125', '0.0625'
]
home       = Path("/home/zvq211")
ds_path    = home / "ds_160_log"
model_path = home / "models"
prog_path  = home / "cfa-raw" / "notebooks" / "train.py"


def write_script(form, ext, noise_type, intensity, n):
    name     = f"{form}{noise_type}{intensity}_{n}"
    filename = f"{name}.sh"
    commands = [
        ["#!/usr/bin/env bash"],
        ["#SBATCH --ntasks=1 --gres=gpu:titanrtx --cpus-per-task=8 --mem=18000M --time=10:00:00"],
        [f"source {home / 'cfa-raw'}/.venv/bin/activate"],
        [
            f"python3.11 {prog_path}",
            f"--clean {ds_path}/png/none/0pct/",
            f"--noise {ds_path}/{form}/{noise_type}/{intensity}pct/",
            f"--output {model_path}",
            f"--model {model_path}/{name}-model.pkl",
            f"--name {name}",
            f"--type .{ext}",
            f"--epochs 100"
        ]
    ]

    with open(filename, "w") as f:
        f.write("\n".join([" ".join(args) for args in commands]))

for (form, ext), noise_type, intensity in product(forms, noise_types, intensities):
    for n in range(5):
        write_script(form, ext, noise_type, intensity, n)
