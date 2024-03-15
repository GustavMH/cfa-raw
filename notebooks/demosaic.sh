#!/usr/bin/env bash

n_threads=$(eval nproc)
n_imgs=9999

path_prefix=tiny-imagenet-200

i=$((2 * n_threads))
time (
for x in $(eval echo {0..$n_imgs}); do
   ((i=i%$((2*n_threads)))); ((i++==0)) && wait
   (dcraw -q 3 -m 3 -c $path_prefix/dng/train_$x.dng | pnmtopng > $path_prefix/ahd/train_$x.png) &
done
)
wait
