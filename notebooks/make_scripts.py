from itertools import product

forms = [("cfa","npy"), ("ahd","png"), ("unproc","png")]
noise_types = ["gaussian", "speckle", "saltpepper"]
intensities = ["10", "30", "70"]

def write_script(form, ext, noise_type, intensity):
    filename = f"{form}{noise_type}{intensity}.sh"
    with open(filename, "w") as f:
        f.write("#!/usr/bin/env bash\n")
        f.write("#SBATCH --ntasks=1 --gres=gpu:titanrtx --cpus-per-task=8 --mem=8000M --time=10:00:00 \n")
        f.write("source cfa-raw/.venv/bin/activate\n")
        f.write(f"python3.11 cfa-raw/notebooks/train.py --clean /home/zvq211/ds_proc/png/none/0pct/ --noise /home/zvq211/ds_proc/{form}/{noise_type}/{intensity}pct/ --output /home/zvq211/models --name {form}{noise_type}{intensity} --type .{ext} --epochs 100\n")

for (form, ext), noise_type, intensity in product(forms, noise_types, intensities):
    write_script(form, ext, noise_type, intensity)
