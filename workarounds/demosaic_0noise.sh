#!/usr/bin/env bash
#SBATCH --ntasks=1 --cpus-per-task=12 --time=10:00:00 --mem=6000M

export ds_path=/home/zvq211/ds_160_log
export PATH=/home/zvq211/cfa-raw/dcraw:$PATH

n_threads=$(eval nproc)

cd $ds_path

# copy folder structure
for j in "mw,-m 3" "m,-m 3 -W", "w, ", "d, -W"; do
   # Set the internal field splitter, reset after use
   IFS=","
   set -- $j
   IFS=$' \t\n'

   for quality in 0 1 2 3; do
      (cd dng/none/0pct && find . -type d -exec mkdir -p -- ../../../$1$quality/{} \; ) && wait
   done
done

# Get a path to every .dng file
LINES=$(cd dng/none/0pct && find -type f | sed "s/\.\///;s/\.dng//")

for j in "mw, -m 3" "m, -m 3 -W", "w, ", "d, -W"; do
   # Set the internal field splitter, reset after use
   IFS=","
   set -- $j
   IFS=$' \t\n'
   for quality in 0 1 2 3; do
      i=$((2 * n_threads))
      for path in $LINES; do
         ((i=i%$((2*n_threads)))); ((i++==0)) && wait
         (dcraw -q $quality -T -g 1 1 $2 dng/none/0pct/$path.dng) \
            && mv dng/none/0pct/$path.tiff $1$quality/$path.tiff &
      done
      wait
   done
done

