#!/usr/bin/env python3

from pidng.core import RAW2DNG, DNGTags, Tag
from pidng.defs import *

# Test Colour Correction Matrix - 1
ccm1 = [[10000,10000], [10000,10000], [10000,10000],
        [10000,10000], [10000,10000], [10000,10000],
        [10000,10000], [10000,10000], [10000,10000]]

# Test Colour Correction Matrix - 2
# Leads to errors
ccm1 = [[0,1], [0,1], [0,1],
        [0,1], [0,1], [0,1],
        [0,1], [0,1], [0,1]]

def saveFile(cfa_img, filename, bpp = 16):
    height, width = cfa_img.shape

    # set DNG tags.
    t = DNGTags()
    #t.set(Tag.ColorMatrix1, ccm1) # colour correction matrix

    t.set(Tag.ImageWidth, width)
    t.set(Tag.ImageLength, height)
    #t.set(Tag.TileWidth, width) # Not supported?
    #t.set(Tag.TileLength, height) # Not supported?
    t.set(Tag.Orientation, Orientation.Horizontal)
    t.set(Tag.PhotometricInterpretation, PhotometricInterpretation.Color_Filter_Array)
    t.set(Tag.SamplesPerPixel, 1)
    t.set(Tag.BitsPerSample, bpp)
    t.set(Tag.CFARepeatPatternDim, [2,2])
    t.set(Tag.CFAPattern, CFAPattern.GBRG)
    #t.set(Tag.BlackLevel, (4096 >> (16 - bpp))) # def. 0
    #t.set(Tag.WhiteLevel, ((1 << bpp) -1)) # def. 2^bpp -1

    #t.set(Tag.CalibrationIlluminant1, CalibrationIlluminant.D65) # def. unknown
    t.set(Tag.AsShotNeutral, [[1,1],[1,1],[1,1]]) # RGB white
    #t.set(Tag.BaselineExposure, [[1, 0]]) # Exposure compensation, defualt 0, repr. 0/1?

    t.set(Tag.Make, "No camera - Synthetically generated")
    t.set(Tag.Model, "v1")
    t.set(Tag.DNGVersion, DNGVersion.V1_4)
    t.set(Tag.DNGBackwardVersion, DNGVersion.V1_2)
    t.set(Tag.PreviewColorSpace, PreviewColorSpace.sRGB)

    # save to dng file.
    r = RAW2DNG()
    r.options(t, path="", compress=False) # Enabling compression crashes Jupyter
    r.convert(cfa_img, filename=filename)
