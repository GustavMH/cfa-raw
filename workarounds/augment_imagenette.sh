#!/usr/bin/env sh
#SBATCH --ntasks=1 --cpus-per-task=16 --mem=5000M --time=1:05:00

source cfa-raw/.venv/bin/activate
which python3.11

nproc
free
lscpu

echo "STARTING DATASET AUGMENTATION"

python3.11 cfa-raw/notebooks/augment_dataset.py --input /home/zvq211/imagenette2-160 --output /home/zvq211/ds_160_log --size 160 64 32.0 16.0 8.0 4.0 2.0 1.0 0.5 0.25 0.125 0.0625
