struct Img {
    h: i32,
    w: i32,
    channels: i32,
    data: &[f32]
};

impl std::ops::Index<(usize, usize)> for Img {
    fn index(&self, (row, col): (usize, usize)) -> f32 {
        &self.data[(row * w + col)]
    }
}

#[inline]
fn fir_hor(i: &Img, col, row) {
    /* Approximation of zØ-FIR filter h */
    return 0.25 * ((pix[-1] + pix[0] + pix[1]) * 2 - pix[-2] - pix[2]);
}

#[inline]
fn fir_ver(*f32 pix) {
    /* Approximation of zØ-FIR filter h */
    return 0.25 * ((pix[-width] + pix[0] + pix[width]) * 2 - pix[-2 * width] - pix[2 * width]) ;
}

fn demosaic(sensor: Img) {
    let TS = 144;
    let width  = W;
    let height = H;

    let xyz_rgb: [[f32; 3]; 3] = [
        /* XYZ from RGB */
        [ 0.412453, 0.357580, 0.180423 ],
        [ 0.212671, 0.715160, 0.072169 ],
        [ 0.019334, 0.119193, 0.950227 ]
    ];

    let d65_white: [f32; 3] = [0.950456, 1., 1.088754];

    for (let top = 2; top < height - 5; top += TS - 6) {
        for (let left = 2; left < width - 5; left += TS - 6) {
            //  Interpolate green horizontally and vertically:
            for (let row = top; row < top + TS && row < height - 2; row++) {
                for (let col = left + (fc(cfa, row, left) & 1); col < std::min(left + TS, width - 2); col += 2) {
                    let pixel_pos = &rawData[row][col];
                    let val0 =
                    rgb[0][row - top][col - left][1] = median(val0, pix[-1], pix[1]);
                    let val1 =
                    rgb[1][row - top][col - left][1] = median(val1, pix[-width], pix[width]);
                }
            }

            //  Interpolate red and blue, and convert to CIELab:
            for (int d = 0; d < 2; d++)
                for (int row = top + 1; row < top + TS - 1 && row < height - 3; row++) {
                    int cng = fc(cfa, row + 1, fc(cfa, row + 1, 0) & 1);
                    for (int col = left + 1; col < std::min(left + TS - 1, width - 3); col++) {
                        auto pix = &rawData[row][col];
                        auto rix = &rgb[d][row - top][col - left];
                        auto lix = lab[d][row - top][col - left];
                        if (fc(cfa, row, col) == 1) {
                            rix[0][2 - cng] = CLIP(pix[0] + (0.5f * (pix[-1] + pix[1]
                                                                     - rix[-1][1] - rix[1][1] ) ));
                            rix[0][cng] = CLIP(pix[0] + (0.5f * (pix[-width] + pix[width]
                                                                 - rix[-TS][1] - rix[TS][1])));
                            rix[0][1] = pix[0];
                        } else {
                            rix[0][cng] = CLIP(rix[0][1] + (0.25f * (pix[-width - 1] + pix[-width + 1]
                                                                     + pix[+width - 1] + pix[+width + 1]
                                                                     - rix[-TS - 1][1] - rix[-TS + 1][1]
                                                                     - rix[+TS - 1][1] - rix[+TS + 1][1])));
                            rix[0][2 - cng] = pix[0];
                        }
                        float xyz[3] = {};

                        for(unsigned int c = 0; c < 3; ++c) {
                            xyz[0] += xyz_cam[0][c] * rix[0][c];
                            xyz[1] += xyz_cam[1][c] * rix[0][c];
                            xyz[2] += xyz_cam[2][c] * rix[0][c];
                        }

                        xyz[0] = cbrt[xyz[0]];
                        xyz[1] = cbrt[xyz[1]];
                        xyz[2] = cbrt[xyz[2]];

                        lix[0] = 116.f * xyz[1] - 16.f;
                        lix[1] = 500.f * (xyz[0] - xyz[1]);
                        lix[2] = 200.f * (xyz[1] - xyz[2]);
                    }
                }

            //  Build homogeneity maps from the CIELab images:

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
                        red[row][col] = rgb[dir][tr][tc][0];
                        green[row][col] = rgb[dir][tr][tc][1];
                        blue[row][col] = rgb[dir][tr][tc][2];
                    } else {
                        red[row][col] = 0.5f * (rgb[0][tr][tc][0] + rgb[1][tr][tc][0]);
                        green[row][col] = 0.5f * (rgb[0][tr][tc][1] + rgb[1][tr][tc][1]);
                        blue[row][col] = 0.5f * (rgb[0][tr][tc][2] + rgb[1][tr][tc][2]);
                    }
                }
            }
        }
