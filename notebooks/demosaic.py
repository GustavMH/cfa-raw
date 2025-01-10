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

def color_conv_mat(xyz, mat):
    i = mat[0,0] * xyz[0] + mat[1,0] * xyz[1] + mat[2,0] * xyz[2]
    j = mat[0,1] * xyz[0] + mat[1,1] * xyz[1] + mat[2,1] * xyz[2]
    k = mat[0,2] * xyz[0] + mat[1,2] * xyz[1] + mat[2,2] * xyz[2]

    return t.stack([i,j,k], dim=-3)

def xyz2lab(xyz):
    res = t.zeros(xyz.shape)
    crt = t.pow(xyz, 1/3)
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
        [ .5,  0,  .5],
        [.25, .5, .25]]
K_G  = [[  0,.25,   0],
        [.25,  0, .25],
        [  0,.25,   0]]

def bilinear(img):
    kernel = t.Tensor([
        [K_RB, K_Z,  K_Z],
        [ K_Z, K_G,  K_Z],
        [ K_Z, K_Z, K_RB]
    ])

    res  = img.clone().unsqueeze(0)
    res += F.conv2d(res, weight=kernel, padding=1)
    return res.squeeze(0)

def ahd(img):
    # AHD assumes the CIErgb colorspace
    c, h, w = img.shape

    v_kf = t.tensor([[[[-1,2,2,2,-1]]]]) / 4
    h_kf = v_kf.transpose(2, 3)

    res = img.clone().unsqueeze(0).unsqueeze(0)

    # Horizontal/vertical interpolation
    G_vb = t.zeros((1,1,h,w))
    G_vb[:, :, 0::2, 1::2] += F.conv2d(res[:,:,1,0:,1:], weight=v_kf, padding=[0,2], stride=2)
    G_vr = t.zeros((1,1,h,w))
    G_vr[:, :, 1::2, 0::2] += F.conv2d(res[:,:,1,1:,0:], weight=v_kf, padding=[0,2], stride=2)

    G_hb = t.zeros((1,1,h,w))
    G_hb[:, :, 0::2, 1::2] += F.conv2d(res[:,:,1,0:,1:], weight=h_kf, padding=[2,0], stride=2)
    G_hr = t.zeros((1,1,h,w))
    G_hr[:, :, 1::2, 0::2] += F.conv2d(res[:,:,1,1:,0:], weight=h_kf, padding=[2,0], stride=2)

    # Candidate red-green, blue-green difference pixels
    Rv  = res[:,:,0] - G_vr
    Rv += F.conv2d(Rv, t.tensor([[K_RB]]), padding=1)
    Bv  = res[:,:,2] - G_vb
    Bv += F.conv2d(Bv, t.tensor([[K_RB]]), padding=1)
    Gv  = res[:,:,1] + G_vb + G_vr
    fv  = t.stack([Gv+Rv, Gv, Gv+Bv], dim=2)

    Rh  = res[:,:,0] - G_hr
    Rh += F.conv2d(Rh, t.tensor([[K_RB]]), padding=1)
    Bh  = res[:,:,2] - G_hb
    Bh += F.conv2d(Bh, t.tensor([[K_RB]]), padding=1)
    Gh  = res[:,:,1] + G_hr + G_hb
    fh  = t.stack([Gh+Rh, Gh, Gh+Bh], dim=2)

    # Homogeneity maps
    # CIErgb -> CIELab
    fv_lab = xyz2lab(color_conv_mat(fv[0,0], rgb2xyz))
    fh_lab = xyz2lab(color_conv_mat(fh[0,0], rgb2xyz))

    # delta = 1, L2
    dir_kf = sparse_ones((4,3,3),[(0,1,0),(1,0,1),(2,2,1),(3,1,2)])

    return t.clamp(fh_lab, 0, 1).squeeze(0).squeeze(0)

img = (pil_to_tensor(Image.open("./unknown.png")).to(t.float32)) / 255
cfa = rgb_to_cfa(img)
dem = np.array(ahd(cfa))
Image.fromarray((np.array(dem) * 255).transpose(1,2,0).astype(np.uint8)).resize((400,400), 0).save("ex.png")
