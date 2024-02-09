/* Ported from RawTherapee
 *
 * Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 * Under the GPL
 * */

use std::{max, min};

struct Img {
    height: usize,
    width: usize,
    channels: i32,
    data: &[f32]
};

impl std::ops::Index<(usize, usize)> for Img {
    fn index(&self, (row, col): (usize, usize)) -> f32 {
        &self.data[(row * w + col)]
    }
}

#[inline]
fn fir_h(i: &Img, c: usize /* column */, r: usize /* row */) {
    /* Approximation of Ø-FIR filter h
     * horizontal direction, see p. 362 */
    return .25 * ((i[r][c-1]+i[r][c]+i[r][c+1]) * 2 - i[r][c-2] - i[r][c+2]);
}

#[inline]
fn fir_v(i: &Img, c: usize /* column */, r: usize /* row */) {
    /* Approximation of Ø-FIR filter h
     * vertical direction, see p. 362 */
    return .25 * ((i[r-1][c]+i[r][c]+i[r+1][c]) * 2 - i[r-2][c] - i[r+2][c]);
}

#[inline]
fn median(a: f32, b: f32, c: f32) {
    /* returns the median between three values
     * This median implementations from Flössie and Ingo Weyrich are inspired by this work:
     *
     * http://ndevilla.free.fr/median/median.pdf
     * http://pages.ripco.net/~jgamble/nw.html
     * https://github.com/hoytech/Algorithm-Networksort-Chooser */

    return max(min(a, b), min(c, max(a, b)));
}

fn convolve(src: Img, dest: Img, kernel: ) {

}

fn demosaic(raw: Img, colStart: i32) {
    let TS = 144;

    let xyz_rgb: [[f32; 3]; 3] = [
        /* XYZ from RGB */
        [ 0.412453, 0.357580, 0.180423 ],
        [ 0.212671, 0.715160, 0.072169 ],
        [ 0.019334, 0.119193, 0.950227 ]
    ];

    let d65_white: [f32; 3] = [0.950456, 1., 1.088754];
    let mut rgb_h = Img::new(sensor.width, sensor.height, 3);
    let mut rgb_v = Img::new(sensor.width, sensor.height, 3);
    let mut rgb = [rgb_h, rgb_v]

    /* Interpolate green horizontally and vertically: */
    for (let row = 2; row < height - 2; row += 1) {
        for (let col = 2; width - 2; col += 2) {
            rgb_h[row][col][1] =
                median(fir_hor(&rawData, row, col), raw[row][col-1], raw[row][col+1]);
            rgb_v[row][col][1] =
                median(fir_ver(&rawData, row, col), raw[row-1][col], raw[row+1][col]);
        }
    }

    /* Interpolate red and blue using bilinear interpolation */
    for (let row = top + 1; row < height - 3; row++) {
        for (let col = left + 1; col < width - 3; col++) {

            auto rix = &rgb[d][row - top][col - left];

            if (fc(cfa, row, col) == 1) {
                rgb[d][row - top][col - left + 2] = CLIP(pix[0] + (0.5 * (pix[-1] + pix[1] - rix[-1][1] - rix[1][1])));
                rgb[d][row - top][col - left + 1] = raw[row-top][col-left][0];
                rgb[d][row - top][col - left]     = CLIP(pix[0] + (0.5 * (pix[-width] + pix[width] - rix[-TS][1] - rix[TS][1])));
            } else {
                rgb[d][row - top][col - left]     = CLIP(rix[0][1] + (0.25 * (pix[-width - 1] + pix[-width + 1] + pix[+width - 1] + pix[+width + 1] - rix[-TS - 1][1] - rix[-TS + 1][1] - rix[+TS - 1][1] - rix[+TS + 1][1])));
                rgb[d][row - top][col - left + 2] = pix[0];
            }
        }
    }

    /* Convert to CIElab */
    for (let row = top + 1; row < height - 3; row++) {
        for (let col = left + 1; col < width - 3; col++) {
            auto lix = lab[d][row - top][col - left];

            float xyz[3] = {};

            for(unsigned int c = 0; c < 3; ++c) {
                xyz[0] += xyz_cam[0][c] * rix[0][c];
                xyz[1] += xyz_cam[1][c] * rix[0][c];
                xyz[2] += xyz_cam[2][c] * rix[0][c];
            }

            xyz[0] = cbrt[xyz[0]];
            xyz[1] = cbrt[xyz[1]];
            xyz[2] = cbrt[xyz[2]];

            lix[0] = 116 *  xyz[1] - 16;
            lix[1] = 500 * (xyz[0] - xyz[1]);
            lix[2] = 200 * (xyz[1] - xyz[2]);
        }
    }

    /*  Build homogeneity maps from the CIELab images: */
    for (int row = top + 2; row < top + TS - 2 && row < height - 4; row++) {
        int tr = row - top;
        float ldiff[2][4], abdiff[2][4];

        for (int col = left + 2, tc = 2; col < left + TS - 2 && col < width - 4; col++, tc++) {
            for (int d = 0; d < 2; d++) {
                auto lix = &lab[d][tr][tc];

                for (int i = 0; i < 4; i++) {
                    ldiff[d][i] = std::fabs(lix[0][0] - lix[dirs[i]][0]);
                    abdiff[d][i] = SQR(lix[0][1] - lix[dirs[i]][1])
                        + SQR(lix[0][2] - lix[dirs[i]][2]);
                }
            }

            float leps = std::min(std::max(ldiff[0][0], ldiff[0][1]),
                                  std::max(ldiff[1][2], ldiff[1][3]));
            float abeps = std::min(std::max(abdiff[0][0], abdiff[0][1]),
                                   std::max(abdiff[1][2], abdiff[1][3]));

            for (int d = 0; d < 2; d++) {
                homo[d][tr][tc] = 0;
                for (int i = 0; i < 4; i++) {
                    homo[d][tr][tc] += (ldiff[d][i] <= leps) * (abdiff[d][i] <= abeps);
                }
            }
        }
    }

            //  Combine the most homogeneous pixels for the final result:
            for (int row = top + 3; row < top + TS - 3 && row < height - 5; row++) {
                int tr = row - top;

                for (int col = left + 3, tc = 3; col < std::min(left + TS - 3, width - 5); col++, tc++) {
                    uint16_t hm0 = 0, hm1 = 0;
                    for (int i = tr - 1; i <= tr + 1; i++)
                        for (int j = tc - 1; j <= tc + 1; j++) {
                            hm0 += homo[0][i][j];
                            hm1 += homo[1][i][j];
                        }

                    if (hm0 != hm1) {
                        int dir = hm1 > hm0;
                        red  [row][col] = rgb[dir][tr][tc][0];
                        green[row][col] = rgb[dir][tr][tc][1];
                        blue [row][col] = rgb[dir][tr][tc][2];
                    } else {
                        red  [row][col] = .5 * (rgb[0][tr][tc][0] + rgb[1][tr][tc][0]);
                        green[row][col] = .5 * (rgb[0][tr][tc][1] + rgb[1][tr][tc][1]);
                        blue [row][col] = .5 * (rgb[0][tr][tc][2] + rgb[1][tr][tc][2]);
                    }
                }
            }
        }
