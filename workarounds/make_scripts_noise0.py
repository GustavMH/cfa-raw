#!/usr/bin/env python3
from pathlib   import Path

home       = Path("/home/zvq211")
ds_path    = home / "ds_proc"
model_path = home / "models"
prog_path  = home / "cfa-raw" / "notebooks" / "train.py"


def write_script(ds, ext, path, n, epochs):
    name     = f"{ds}_{path}_{n}"
    filename = f"{name}.sh"
    commands = [
        ["#!/usr/bin/env bash"],
        [f"source {home / 'cfa-raw'}/.venv/bin/activate"],
        [
            f"python3.11 {prog_path}",
            f"--clean {ds_path}/{ds}/png",
            f"--noise {ds_path}/{ds}/{path}",
            f"--output {model_path}",
            f"--model {model_path}/{name}-model.pkl",
            f"--name {name}",
            f"--type .{ext}",
            f"--epochs {epochs}"
        ]
    ]

    with open(filename, "w") as f:
        f.write("\n".join([" ".join(args) for args in commands]))

for ds, epochs in [("CelebA", 100), ("imagenette2-160",300), ("GTSRB",100)]:
    for path in ["a","aw","amw","m", "mw", "w", "d"]:
        for i in range(4):
            for n in range(5):
                write_script(ds, "tiff", f"{path}{i}", n, epochs)
