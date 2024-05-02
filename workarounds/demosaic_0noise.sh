#!/usr/bin/env bash
#SBATCH --ntasks=1 --cpus-per-task=12 --time=10:00:00 --mem=6000M

export ds_path=~/Downloads/ds_proc/noise0
export PATH=/home/zvq211/cfa-raw/dcraw:$PATH

n_threads=$(eval nproc)

cd $ds_path

for quality in 0 1 2 3; do
   (cd dng && find . -type d -exec mkdir -p -- ../d$quality/{} \; ) && wait
done

# copy folder structure
for quality in 0 1 2 3; do
   i=$((2 * n_threads))
   for path in $(cd dng && find -type f | sed "s/\.\///;s/\.dng//"); do
      ((i=i%$((2*n_threads)))); ((i++==0)) && wait
      (dcraw -q $quality -T -g 1 1 dng/$path.dng) && mv dng/$path.tiff d$quality/$path.tiff &
   done
   wait
done
