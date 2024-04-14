#!/usr/bin/env python3
from itertools import product

forms = [("npy","npy"), ("ahd","tiff"), ("png","png")]
noise_types = ["gaussian", "speckle", "saltpepper"]
intensities = [
    '64pct', '32.0pct', '16.0pct', '8.0pct',
    '4.0pct', '2.0pct', '1.0pct', '0.5pct',
    '0.25pct', '0.125pct', '0.0625pct'
]
ds_path = "/home/zvq211/ds_160_log"
model_path = "/home/zvq211/models"


def write_script(form, ext, noise_type, intensity, n):
    filename = f"{form}{noise_type}{intensity}_{n}.sh"
    with open(filename, "w") as f:
        # This section should be rewritten
        # with pathlib and a list of args so whitespace is automatic
        f.write("#!/usr/bin/env bash\n")
        f.write("#SBATCH --ntasks=1 --gres=gpu:titanrtx --cpus-per-task=8 --mem=8000M --time=10:00:00 \n")
        f.write("source cfa-raw/.venv/bin/activate\n")
        f.write(f"python3.11 cfa-raw/notebooks/train.py ")
        f.write(f"--clean {ds_path}/png/none/0pct/ ")
        f.write(f"--noise {ds_path}/{form}/{noise_type}/{intensity}pct/ ")
        f.write(f"--output {model_path} ")
        f.write(f"--model {model_path}/{form}{noise_type}{intensity}_{n}-model.pkl")
        f.write(f"--name {form}{noise_type}{intensity}_{n} ")
        f.write(f"--type .{ext} ")
        f.write(f"--epochs 100\n")

for (form, ext), noise_type, intensity in product(forms, noise_types, intensities):
    for n in range(5):
        write_script(form, ext, noise_type, intensity, n)
