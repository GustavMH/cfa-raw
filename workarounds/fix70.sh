#!/usr/bin/env sh
#SBATCH --ntasks=1 --gres=gpu:titanrtx --cpus-per-task=8 --mem=22000M --time=10:00:00

source /home/zvq211/cfa-raw/.venv/bin/activate
python3.11 ~/cfa-raw/notebooks/train.py --clean ~/ds_160/png/none/0pct/ --noise ~/ds_160/png/speckle/70pct/ --output ~/models/ --name "pngspeckle70" --type ".png" --epochs 100 --model ./pngspeckle70-model.pkl
