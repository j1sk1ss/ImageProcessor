#include "../include/filters.h"


int FILTER_apply_matrix(bitmap_t* bmp, matrix_t* mtrx) {
    if (!bmp || !mtrx) return -1;

    int width  = bmp->iheader.biWidth;
    int height = bmp->iheader.biHeight;
    int half_x = mtrx->x / 2;
    int half_y = mtrx->y / 2;

    bitmap_t copy;
    BMP_copy(bmp, &copy);

    palette_t p, new_p;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sum_r = 0, sum_g = 0, sum_b = 0;
            double weight = 0;

            for (int dy = -half_y; dy <= half_y; dy++) {
                for (int dx = -half_x; dx <= half_x; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx < 0) nx = 0;
                    if (ny < 0) ny = 0;
                    if (nx >= width) nx = width - 1;
                    if (ny >= height) ny = height - 1;

                    BMP_get_pixel_24(nx, ny, &copy, &p);
                    double w = (double)mtrx->body[dy + half_y][dx + half_x];
                    sum_r += p.r * w;
                    sum_g += p.g * w;
                    sum_b += p.b * w;
                    weight += w;
                }
            }

            if (weight == 0) weight = 1;
            new_p.r = (unsigned char) MAX(0, MIN(255, sum_r / weight));
            new_p.g = (unsigned char) MAX(0, MIN(255, sum_g / weight));
            new_p.b = (unsigned char) MAX(0, MIN(255, sum_b / weight));
            BMP_set_pixel_24(x, y, bmp, &new_p);
        }
    }

    return 1;
}
