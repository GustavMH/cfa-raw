#!/usr/bin/env python3

import numpy as np
from cfa import *

import torch.nn.functional as F
import torch
from torch import Tensor

rgb_kf  = sparse_ones((3,2,2), [(0,0,1),(1,0,0),(2,1,0),(1,1,1)])

bayer_k = Tensor(sparse_ones((4,1,2,2), [(0,0,0,1),(1,0,0,0),(2,0,1,0),(3,0,1,1)]))
rgb_k   = Tensor(sparse_ones((4,3,2,2), [(0,0,0,1),(1,1,0,0),(2,2,1,0),(3,1,1,1)]))

def _bayer_to_rgb(img):
    # seperates colour components to separate layers
    tensor = torch.Tensor(img).unsqueeze(0)
    conv   = F.conv2d(        tensor, stride=2, weight=bayer_k, padding=1)
    return F.conv_transpose2d(conv,   stride=2, weight=rgb_k,   padding=1).permute(1,2,0)

def _rgb_to_bayer(img):
    # removes 2/3 of photosite information
    tensor = torch.Tensor(img).unsqueeze(0)
    conv   = F.conv2d(        tensor, stride=2, weight=rgb_k, padding=1)
    return F.conv_transpose2d(conv,   stride=2, weight=rgb_k, padding=1)

def test_rgb_to_cfa():
    im = np.random.randint(1000, size=(200, 200, 3), dtype=np.int16)
    rgb_index = rgb_to_cfa(np.array(im), rgb_kf)
    rgb_conv  = _rgb_to_bayer(torch.tensor(np.array(im)).to(torch.float32).permute(2,0,1)).squeeze().sum(dim=0)
    assert np.abs(rgb_index - np.array(rgb_conv)).sum() == 0, "The reference rgb_to_cfa doesn't match module version"

if __name__ == '__main__':
    test_rgb_to_cfa()
    print("tests passed!")
