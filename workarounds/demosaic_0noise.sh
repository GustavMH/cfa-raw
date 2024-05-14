#!/usr/bin/env bash
#SBATCH --ntasks=1 --cpus-per-task=12 --time=10:00:00 --mem=6000M

export ds_path=/home/zvq211/ds_160_log
export PATH=/home/zvq211/cfa-raw/dcraw:$PATH

n_threads=$(eval nproc)

cd $ds_path

for j in "mw,-m 3" "m,-m 3 -W", "w, ", "d, -W"; do
   set -- $j
   for quality in 0 1 2 3; do
      (cd dng/none/0pct && find . -type d -exec mkdir -p -- ../../../$1$quality/{} \; ) && wait
   done
done

# copy folder structure
for j in "mw,-m 3" "m,-m 3 -W", "w, ", "d, -W"; do
   set -- $j
   for quality in 0 1 2 3; do
      i=$((2 * n_threads))
      for path in $(cd dng/none/opct && find -type f | sed "s/\.\///;s/\.dng//"); do
         ((i=i%$((2*n_threads)))); ((i++==0)) && wait
         (dcraw -q $quality -T -g 1 1  dng/none/0pct/$path.dng) && mv dng/none/0pct/$path.tiff $d$quality/$path.tiff &
      done
      wait
   done
done
