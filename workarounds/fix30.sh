#!/usr/bin/env sh
#SBATCH --ntasks=1 --gres=gpu:titanrtx --cpus-per-task=8 --mem=22000M --time=10:00:00

source /home/zvq211/cfa-raw/.venv/bin/activate
python3.11 ~/cfa-raw/notebooks/train.py --clean ~/ds_160/png/none/0pct/ --noise ~/ds_160/ahd/gaussian/30pct/ --output ~/models/ --name "ahdgaussian30" --type ".tiff" --model ./ahdgaussian30-model.pkl
