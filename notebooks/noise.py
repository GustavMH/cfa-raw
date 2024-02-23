#!/usr/bin/env python3

import numpy as np

def salt_pepper_noise(img, density=.3, sp_ratio=.5, seed=[1,2,3]):
    h, w, c = img.shape
    d_max   = np.iinfo(img.dtype).max

    n_speckles = w*h*density
    n_salt     = int(n_speckles*sp_ratio)
    n_pepper   = int(n_speckles*(1-sp_ratio))

    gen = np.random.default_rng(seed)

    # What if the integers overlap?
    def rand_coords(n):
        unique_rand = np.random.choice(range(w*h), size=n, replace=False)
        return [(x % h, x // h) for x in unique_rand]

    res = img.copy()

    for coord in rand_coords(n_salt):
        res[coord] = d_max

    for coord in rand_coords(n_pepper):
        res[coord] = 0

    return res


def gaussian_noise(img, bias=.0, scale=.1, input_scale=1, seed=[1,2,3]):
    h, w, c = img.shape
    d_max   = np.iinfo(img.dtype).max

    gen     = np.random.default_rng(seed)
    noise   = np.expand_dims(gen.normal(bias * d_max, scale * d_max, (h, w)), 2)

    res = input_scale * img.astype(np.int64) + noise.astype(np.int64)

    return np.clip(res, 0, d_max).astype(img.dtype)
