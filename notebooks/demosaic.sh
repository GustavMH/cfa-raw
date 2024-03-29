#!/usr/bin/env bash
#SBATCH --ntasks=1 --cpus-per-task=12 --time=10:00:00 --mem=6000M

export ds_path=/home/zvq211/ds_160
export PATH=/home/zvq211/cfa-raw/dcraw:$PATH

n_threads=$(eval nproc)

cd $ds_path

# copy folder structure
(cd dng && find . -type d -exec mkdir -p -- ../ahd/{} \; ) && wait

i=$((2 * n_threads))
time (
for path in $(cd dng && find -type f | sed "s/\.\///;s/\.dng//"); do
   ((i=i%$((2*n_threads)))); ((i++==0)) && wait
   (dcraw -q 3 -m 3 -T dng/$path.dng) && mv dng/$path.tiff ahd/$path.tiff &
done
)
wait
