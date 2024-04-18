#!/usr/bin/env sh

# check overview for failing jobs, 6 jobs, 5 of same type, rerun the last
sacct --format state,jobid,cputime

# check what job is failing, 0.125pct gaussian noise
find ~/models/scripts-gamma/ -name "*.sh" | sort | sed "6q;d"

# Find file no. difference between .dng and tiff files
ls ds_160_log/ahd-no-gamma/gaussian/0.125pct/val | wc -l
ls ds_160_log/dng/gaussian/0.125pct/val | wc -l

# Check that files arent empty, find 834.tiff to be empty
find ds_160_log/ahd-no-gamma/gaussian/0.125pct/val -type f -size -1000c

# Demosaic file
./cfa-raw/dcraw/dcraw -q 3 -g 1 1 -T -m 3 ds_160_log/dng/gaussian/0.125pct/val/834.dng
mv ds_160_log/dng/gaussian/0.125pct/val/834.tiff ds_160_log/ahd-no-gamma/gaussian/0.125pct/val/834.tiff
