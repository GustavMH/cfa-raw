#!/usr/bin/env python3

import numpy as np
import torch

def mult_speckle_noise(img, amount=.3):
    # for J = I + n*I many often use Gauss distributed n
    # rather than the real exponential distribution
    h, w, c = img.shape
    d_max   = np.iinfo(img.dtype).max

    gen     = np.random.default_rng()
    noise   = np.expand_dims(gen.exponential(1/amount, (h, w)), 2)

    res     = img.astype(np.int64) * noise.astype(np.int64)

    return np.clip(res, 0, d_max).astype(img.dtype)

def salt_pepper_noise(img, amount=.3, sp_ratio=.5, seed=[1,2,3]):
    h, w, c = img.shape
    d_max   = np.iinfo(img.dtype).max

    n_speckles = w*h*amount
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


def gaussian_noise(img, bias=.0, amount=.1, input_scale=1):
    h, w, c = img.shape
    d_max   = np.iinfo(img.dtype).max

    gen     = np.random.default_rng()
    noise   = np.expand_dims(gen.normal(bias * d_max, amount * d_max, (h, w)), 2)

    res = input_scale * img.astype(np.int64) + noise.astype(np.int64)

    return np.clip(res, 0, d_max).astype(img.dtype)


def torch_type_helper(tensor):
    if torch.is_floating_point(tensor):
        return torch.finfo, torch.float64
    elif not torch.is_complex(tensor):
        return torch.iinfo, torch.int64
    else:
        raise "Not float or int"


def gaussian_noise_torch(img, bias=.0, std=.1, input_scale=1, seed=[1,2,3], device="cpu"):
    info, double_type = torch_type_helper(img)

    c, h, w = img.shape
    d_max   = info(img.dtype).max

    gen     = torch.Generator(device=device)
    noise   = torch.normal(bias * d_max, std * d_max, (h, w), generator=gen).expand(c, h, w)

    res = input_scale * img.to(double_type) + noise.to(double_type)

    return np.clip(res, 0, d_max).to(img.dtype)


def salt_pepper_noise_torch(img, density=.3, sp_ratio=.5, seed=[1,2,3], device="cpu"):
    info, double_type = torch_type_helper(img)

    c, h, w = img.shape
    d_max   = info(img.dtype).max

    n_speckles = w*h*density
    n_salt     = int(n_speckles*sp_ratio)
    n_pepper   = int(n_speckles*(1-sp_ratio))

    gen = torch.Generator(device=device)

    def rand_coords(n):
        unique_rand = torch.randperm(h*w, generator=gen)[:n]
        return [(x % h, x // h) for x in unique_rand]

    res = img.clone()

    for x, y in rand_coords(n_salt):
        res[:, x, y] = d_max

    for x, y in rand_coords(n_pepper):
        res[:, x, y] = 0

    return res
