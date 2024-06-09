---
author: Frederik Altmann & Gustav Mark-Hansen
---
# CFA-RAW

Code for generating CFA data, and training U-Net models.


### Models
Denoising U-net model: [notebooks/denoisingunet.py](notebooks/denoisingunet.py)

Training loop: [notebooks/train.py](notebooks/train.py)

### Datasets
noise augmentation + .dng convertion: [notebooks/ds_augment.py](notebooks/ds_augment.py)

.dng convertion: [notebooks/ds_convert.py](notebooks/ds_convert.py)

### Misc.
Slurm specific scripts: [workarounds/](workarounds/)
