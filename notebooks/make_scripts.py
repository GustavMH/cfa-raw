from itertools import product

forms = [("npy","npy"), ("ahd","tiff"), ("png","png")]
noise_types = ["gaussian", "speckle", "saltpepper"]
intensities = [
    "0.00195312pct", "0.0078125pct", "0.03125pct",
    "0.125pct", "0.5pct", "20pct", "40pct", "60pct",
    "80pct", "0.00390625pct", "0.015625pct", "0.0625pct",
    "0.25pct", "10pct", "30pct", "50pct", "70pct", "90pct"
]

def write_script(form, ext, noise_type, intensity, n):
    filename = f"{form}{noise_type}{intensity}_{n}.sh"
    with open(filename, "w") as f:
        f.write("#!/usr/bin/env bash\n")
        f.write("#SBATCH --ntasks=1 --gres=gpu:titanrtx --cpus-per-task=8 --mem=8000M --time=10:00:00 \n")
        f.write("source cfa-raw/.venv/bin/activate\n")
        f.write(f"python3.11 cfa-raw/notebooks/train.py")
        f.write(f"--clean /home/zvq211/ds_proc/png/none/0pct/ ")
        f.write(f"--noise /home/zvq211/ds_proc/{form}/{noise_type}/{intensity}pct/ ")
        f.write(f"--output /home/zvq211/models ")
        f.write(f"--model /home/zvq211/models/{form}{noise_type}{intensity}_{n}-model.pkl")
        f.write(f"--name {form}{noise_type}{intensity}_{n} ")
        f.write(f"--type .{ext} ")
        f.write(f"--epochs 100\n")

for (form, ext), noise_type, intensity in product(forms, noise_types, intensities):
    for n in range(10):
        write_script(form, ext, noise_type, intensity, n)
