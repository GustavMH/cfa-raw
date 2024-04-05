#!/usr/bin/env bash
#SBATCH --ntasks=1 --gres=gpu:titanrtx --cpus-per-task=8 --mem=15G --time=10:00:00

bash $(find ~/models/scripts/ -name "*.sh" | sort | sed "$(echo $SLURM_ARRAY_TASK_ID)q;d")
