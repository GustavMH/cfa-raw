#!/usr/bin/env sh

export PATH=/home/zvq211/pnmtopng:/home/zvq211/dcraw:$PATH

cd ds_proc

mkdir ahd

dcraw -q 3 -m 3 -c dng/gaussian/10pct/train/1.dng | pnmtopng > ahd/1.png
