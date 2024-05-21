#!/usr/bin/env sh

cd npy
mkdir val
find celeba_hq_256/ -type f -regex '.*\/2[0-9][0-9][0-9][0-9].npy' -exec mv {} val/ \; &
mv celeba_hq_256 train

cd ../dng
mkdir val
find celeba_hq_256/ -type f -regex '.*\/2[0-9][0-9][0-9][0-9].dng' -exec mv {} val/ \; &
mv celeba_hq_256 train

cd ../png
mkdir val
find celeba_hq_256/ -type f -regex '.*\/2[0-9][0-9][0-9][0-9].png' -exec mv {} val/ \; &
mv celeba_hq_256 train
