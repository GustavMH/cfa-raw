#!/usr/bin/env sh
#sbatch --cpus-per-task=8 --mem=500M

export ds_path=/home/zvq211/ds_160
export PATH=/home/zvq211/cfa-raw/dcraw:$PATH

n_threads=$(eval nproc)

cd $ds_path

i=$((2 * n_threads))
time (
for path in 1005 1279 1396 1408 1497 1616 1783 2109 2280 2590 2692 2810 310 3520 3751 636 653 670 834 961; do
   ((i=i%$((2*n_threads)))); ((i++==0)) && wait
   (dcraw -q 3 -m 3 -T dng/gaussian/30pct/val/$path.dng) && mv dng/gaussian/30pct/val/$path.tiff ahd/$path.tiff &
done
)
wait
