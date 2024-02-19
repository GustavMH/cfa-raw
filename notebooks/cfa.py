#!/usr/bin/env python3

import numpy as np



def rgb_to_cfa(img: np.ndarray, kernel: np.ndarray):
    """ Create a 2d np.array conceptually equivalent to
    'conv2d^T(conv2d(img, kernel), kernel).sum(dim=2)'

    Uses the kernel as a "stencil" allowing a sub set of values through."""


    h, w, c    = img.shape
    kc, kw, kh = kernel.shape

    if not c == kc:
        raise Exception("Differing number of kernel and image channels!")

    res = np.zeros((h, w))

    for channel in range(c):
        for off_w, r in enumerate(kernel[channel]):
            for off_h, c in enumerate(r):
                if c == 1:
                    res[off_h::kh, off_w::kw] = img[off_h::kh, off_w::kw, channel]

    return res


def colorize_cfa(img: np.ndarray, kernel: np.ndarray):
    """Used to visualize the sensor pattern

    conceptually equivalent to
    'conv2d^T(conv2d(img, kernel), kernel)'"""

    h, w       = img.shape
    kc, kw, kh = kernel.shape

    res = np.zeros((h, w, kc))

    for channel in range(kc):
        for off_w, r in enumerate(kernel[channel]):
            for off_h, c in enumerate(r):
                if c == 1:
                    res[off_h::kh, off_w::kw, channel] = img[off_h::kh, off_w::kw]

    return res


def sparse_ones(shape: tuple, coords: list):
    """Takes a tuple for SHAPE, and a list of indicies COORDS.

    Returns an array ARR, where ARR.shape = SHAPE and there is a 1 at every
    indicie in COORDS"""

    arr = np.zeros(shape)
    for coor in coords:
        arr[coor] = 1.0

    return arr
