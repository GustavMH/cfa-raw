#!/usr/bin/env python3
import os

import torch.nn.functional as F
import math
from torchvision.transforms.functional import pil_to_tensor, to_pil_image
from PIL import Image
import torch as t
import numpy as np
from scipy.ndimage import median_filter
from itertools import product


srgb2xyz = t.tensor([
    [0.4124564, 0.3575761, 0.1804375],
    [0.2126729, 0.7151522, 0.0721750],
    [0.0193339, 0.1191920, 0.9503041]
])

rgb2xyz= t.tensor([
    [0.49000,0.31000,0.200000],
    [0.17697,0.81240,0.010630],
    [0.00000,0.01000,0.990000]
])

d65_white = t.tensor([ 0.950456, 1, 1.088754 ])

cbrt_lut = t.tensor([math.cbrt(r) if r > 0.008856 else 7.787 * r + 16 / 116.0 for r in t.linspace(0,1,65535)])

def color_conv_mat(xyz, mat):
    i = mat[0,0] * xyz[0] + mat[1,0] * xyz[1] + mat[2,0] * xyz[2]
    j = mat[0,1] * xyz[0] + mat[1,1] * xyz[1] + mat[2,1] * xyz[2]
    k = mat[0,2] * xyz[0] + mat[1,2] * xyz[1] + mat[2,2] * xyz[2]

    return t.stack([i,j,k], dim=-3)

def xyz2lab(xyz):
    res = t.zeros(xyz.shape)

    # TODO LUT
    crt = cbrt_lut.gather(0, (xyz.view(-1) * 65535).to(t.int64)).view(xyz.shape)
    for i, v in enumerate(d65_white):
        crt[i] /= v*100
    res[0] = 166 *  crt[1] - 16
    res[1] = 500 * (crt[0] - crt[1])
    res[2] = 200 * (crt[1] - crt[2])
    return res

def sparse_ones(shape: tuple, coords: list):
    arr = t.zeros(shape)
    for coor in coords:
        arr[coor] = 1.0
    return arr

# Img to CFA
BAYER_KF = sparse_ones((3,2,2), [(0,0,1),(1,0,0),(2,1,0),(1,1,1)])

def rgb_to_cfa(img, kernel=BAYER_KF):
    c, h, w    = img.shape
    kc, kw, kh = kernel.shape

    if not c == kc:
        raise Exception(f"Differing number of kernel and image channels! c: {c}, kc: {kc}")

    res = t.zeros((c, h, w))

    for channel in range(c):
        for off_w, r in enumerate(kernel[channel]):
            for off_h, a in enumerate(r):
                res[channel, off_h::kh, off_w::kw] += img[channel, off_h::kh, off_w::kw] * a

    return res

K_Z  = [[0] * 3] * 3
K_RB = [[.25, .5, .25],
        [ .5,  1,  .5],
        [.25, .5, .25]]
K_G  = [[  0,.25,   0],
        [.25,  1, .25],
        [  0,.25,   0]]

def bilinear(img):
    kernel = t.Tensor([
        [K_RB, K_Z,  K_Z],
        [ K_Z, K_G,  K_Z],
        [ K_Z, K_Z, K_RB]
    ])

    res = F.conv2d(img.clone().unsqueeze(0), weight=kernel, padding=1)
    return res.squeeze(0)

def ahd(img):
    import time

    start = time.time()
    # AHD assumes the CIErgb colorspace
    c, h, w = img.shape

    v_kf = t.tensor([[[[-1,2,2,2,-1]]]]) / 4
    h_kf = v_kf.transpose(2, 3)

    res = img.unsqueeze(0).unsqueeze(0)

    end = time.time()
    print("INIT", end - start)
    start = time.time()
    # Horizontal/vertical interpolation
    G_vb, G_vr, G_hb, G_hr = t.zeros((4,1,1,h,w))
    G_vb[:, :, 0::2, 1::2] += F.conv2d(res[:,:,1,0:,1:], weight=v_kf, padding=[0,2], stride=2)
    G_vr[:, :, 1::2, 0::2] += F.conv2d(res[:,:,1,1:,0:], weight=v_kf, padding=[0,2], stride=2)
    G_hb[:, :, 0::2, 1::2] += F.conv2d(res[:,:,1,0:,1:], weight=h_kf, padding=[2,0], stride=2)
    G_hr[:, :, 1::2, 0::2] += F.conv2d(res[:,:,1,1:,0:], weight=h_kf, padding=[2,0], stride=2)

    end = time.time()
    print("Horizontal/vertical interpolation", end - start)

    start = time.time()
    # Candidate red-green, blue-green difference pixels
    Rv  = F.conv2d(res[:,:,0] - G_vr, t.tensor([[K_RB]]), padding=1)
    Bv  = F.conv2d(res[:,:,2] - G_vb, t.tensor([[K_RB]]), padding=1)
    Gv  = res[:,:,1] + G_vb + G_vr
    fv  = t.stack([Gv+Rv, Gv, Gv+Bv], dim=2)

    Rh  = F.conv2d(res[:,:,0] - G_hr, t.tensor([[K_RB]]), padding=1)
    Bh  = F.conv2d(res[:,:,2] - G_hb, t.tensor([[K_RB]]), padding=1)
    Gh  = res[:,:,1] + G_hr + G_hb
    fh  = t.stack([Gh+Rh, Gh, Gh+Bh], dim=2)

    end = time.time()
    print("Diff pixels", end - start)

    start = time.time()
    # Homogeneity maps
    # CIErgb -> CIELab
    fv_l, fv_a, fv_b = xyz2lab(color_conv_mat(fv[0,0], rgb2xyz)).unsqueeze(1)
    fh_l, fh_a, fh_b = xyz2lab(color_conv_mat(fh[0,0], rgb2xyz)).unsqueeze(1)

    end = time.time()
    print("Color conversion", end - start)

    start = time.time()

    # delta = 1, L2
    dir_kf = (
        sparse_ones((4,3,3),[(0,1,1),(1,1,1),(2,1,1),(3,1,1)]) -
        sparse_ones((4,3,3),[(0,1,0),(1,0,1),(2,2,1),(3,1,2)])
    ).unsqueeze(1)
    fv_ldiff = t.abs(F.conv2d(fv_l, weight=dir_kf, padding=1))
    fv_cdiff = (
        t.square(F.conv2d(fv_a, weight=dir_kf, padding=1)) +
        t.square(F.conv2d(fv_b, weight=dir_kf, padding=1))
    )
    fh_ldiff = t.abs(F.conv2d(fh_l, weight=dir_kf, padding=1))
    fh_cdiff = (
        t.square(F.conv2d(fh_a, weight=dir_kf, padding=1)) +
        t.square(F.conv2d(fh_b, weight=dir_kf, padding=1))
    )

    leps = t.minimum(
        t.maximum(fh_ldiff[0], fh_ldiff[2]),
        t.maximum(fv_ldiff[1], fv_ldiff[3])
    )
    ceps = t.minimum(
        t.maximum(fh_cdiff[0], fh_cdiff[2]),
        t.maximum(fv_cdiff[1], fv_cdiff[3])
    )

    fv_hom = t.logical_and(fv_ldiff <= leps, fv_cdiff <= ceps).sum(0)
    fh_hom = t.logical_and(fh_ldiff <= leps, fh_cdiff <= ceps).sum(0)

    vh_hom = fv_hom >= fh_hom

    res = fv * vh_hom + fh * (~ vh_hom)

    end = time.time()
    print("Homogeneity maps", end - start)

    return t.clamp(res, 0, 1).squeeze(0).squeeze(0)

#img = (pil_to_tensor(Image.open("./unknown.png")).to(t.float32)) / 255
#img = (pil_to_tensor(Image.open("/home/gustav/Pictures/dumb-shit/297.jpeg")).to(t.float32)) / 255
#cfa = rgb_to_cfa(img)
dem = np.array(ahd(cfa))
Image.fromarray((np.array(dem) * 255).transpose(1,2,0).astype(np.uint8)).save("ex.png")
