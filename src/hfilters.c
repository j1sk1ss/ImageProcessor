#include "../include/filters.h"


int FILTER_apply_matrix(bitmap_t* bmp, matrix_t* mtrx) {
    if (!bmp || !mtrx) return -1;

    int width  = bmp->iheader.biWidth;
    int height = bmp->iheader.biHeight;
    int half_x = mtrx->rows / 2;
    int half_y = mtrx->cols / 2;

    bitmap_t copy;
    int copy_res = BMP_copy(bmp, &copy);
    if (copy_res != 1) {
        return -2;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sum_r = 0, sum_g = 0, sum_b = 0, weight = 0;
            for (int dy = -half_y, ky = 0; dy <= half_y; dy++, ky++) {
                int ny = CLAMP(y + dy, 0, height - 1);
                for (int dx = -half_x, kx = 0; dx <= half_x; dx++, kx++) {
                    int nx = CLAMP(x + dx, 0, width - 1);
                    double w = mtrx->body[ky][kx];
                    if (w == IGNORE) continue;

                    palette_t p;
                    BMP_get_pixel_24(nx, ny, &copy, &p);
                    sum_r  += p.r * w;
                    sum_g  += p.g * w;
                    sum_b  += p.b * w;
                    weight += w;
                }
            }

            palette_t new_p = {
                .r = (unsigned char)CLAMP(sum_r, 0, 255),
                .g = (unsigned char)CLAMP(sum_g, 0, 255),
                .b = (unsigned char)CLAMP(sum_b, 0, 255),
            };

            BMP_set_pixel_24(x, y, bmp, &new_p);
        }
    }

    BMP_unload(&copy);
    return 1;
}

int FILTER_apply_axis_matrix(bitmap_t* bmp, matrix_t* x_kern, matrix_t* y_kern) {
    if (!bmp || !x_kern || !y_kern) return -1;

    int width  = bmp->iheader.biWidth;
    int height = bmp->iheader.biHeight;
    int half_x = x_kern->cols / 2;
    int half_y = y_kern->rows / 2;

    bitmap_t copy;
    if (BMP_copy(bmp, &copy) != 1) return -2;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sum_r = 0, sum_g = 0, sum_b = 0;
            for (int dx = -half_x, kx = 0; dx <= half_x; dx++, kx++) {
                int nx = CLAMP(x + dx, 0, width - 1);
                double w = x_kern->body[0][kx];

                palette_t p;
                BMP_get_pixel_24(nx, y, &copy, &p);
                sum_r += p.r * w;
                sum_g += p.g * w;
                sum_b += p.b * w;
            }

            palette_t new_p = {
                .r = (unsigned char)CLAMP(sum_r, 0, 255),
                .g = (unsigned char)CLAMP(sum_g, 0, 255),
                .b = (unsigned char)CLAMP(sum_b, 0, 255),
            };

            BMP_set_pixel_24(x, y, bmp, &new_p);
        }
    }

    BMP_unload(&copy);
    if (BMP_copy(bmp, &copy) != 1) return -2;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sum_r = 0, sum_g = 0, sum_b = 0;
            
            for (int dy = -half_y, ky = 0; dy <= half_y; dy++, ky++) {
                int ny = CLAMP(y + dy, 0, height - 1);
                double w = y_kern->body[ky][0];

                palette_t p;
                BMP_get_pixel_24(x, ny, &copy, &p);
                sum_r += p.r * w;
                sum_g += p.g * w;
                sum_b += p.b * w;
            }

            palette_t new_p = {
                .r = (unsigned char)CLAMP(sum_r, 0, 255),
                .g = (unsigned char)CLAMP(sum_g, 0, 255),
                .b = (unsigned char)CLAMP(sum_b, 0, 255),
            };

            BMP_set_pixel_24(x, y, bmp, &new_p);
        }
    }

    BMP_unload(&copy);
    return 1;
}
