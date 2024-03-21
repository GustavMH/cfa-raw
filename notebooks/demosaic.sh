#!/usr/bin/env bash

n_threads=$(eval nproc)

cd $ds_path

# copy folder structure
(cd dng && find . -type d -exec mkdir -p -- ../ahd/{} \; ) && wait

i=$((2 * n_threads))
time (
for path in $(cd dng && find -type f | sed "s/\.\///;s/\.dng//"); do
   ((i=i%$((2*n_threads)))); ((i++==0)) && wait
   (dcraw -q 3 -m 3 -c dng/$path.dng | pnmtopng > ahd/$path.png) &
done
)
wait
