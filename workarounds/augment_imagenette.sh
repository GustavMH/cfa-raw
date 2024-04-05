#!/usr/bin/env sh
#SBATCH --ntasks=1 --cpus-per-task=16 --mem=5000M --time=1:05:00

source cfa-raw/.venv/bin/activate
which python3.11

nproc
free
lscpu

echo "STARTING DATASET AUGMENTATION"

python3.11 cfa-raw/notebooks/augment_dataset.py --input /home/zvq211/imagenette2-160 --output /home/zvq211/ds_160 --size 160 10 30 70
