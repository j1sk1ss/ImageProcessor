#include <stdio.h>
#include "include/bitmap.h"
#include "include/filters.h"
#include "include/matrix.h"


int main(int argc, char* argv[]) {
    matrix_t gaussian_mtrx;
    MTRX_create_gaussian(&gaussian_mtrx, 5);

    bitmap_t bmp;
    BMP_read("lenna_gs.bmp", &bmp);

    FILTER_apply_matrix(&bmp, &gaussian_mtrx);
    BMP_save("lenna_gaussian.bmp", &bmp);
    BMP_unload(&bmp);
    MTRX_unload(&gaussian_mtrx);
    return 0;
}

int __test_border(const char* path) {
    matrix_t sharp_mtrx;
    MTRX_read("sharp.flt", &sharp_mtrx);

    bitmap_t bmp;
    BMP_read(path, &bmp);

    FILTER_grayscale(&bmp);
    FILTER_apply_matrix(&bmp, &sharp_mtrx);
    FILTER_threshold(&bmp, 128);
    BMP_save("lenna_border.bmp", &bmp);
    BMP_unload(&bmp);
    MTRX_unload(&sharp_mtrx);
    return 1;
}

int __test_default_filters(const char* path) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        return -1;
    }

    for (int i = 0; i < bmp.iheader.biHeight; i++) {
        int set_res = 1;
        for (int j = 0; j < bmp.iheader.biWidth; j++) {
            palette_t plt = {
                .r = 255,
                .g = 255,
                .b = 255
            };

            set_res = BMP_set_pixel_24(j, i, &bmp, &plt);
            if (set_res != 1) {
                printf("BMP_set_pixel_24 error! %i\n", set_res);
                break;
            }
        }

        if (set_res != 1) break;
    }

    const char* new_path = "default_test_filled.bmp";
    int save_res = BMP_save(new_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
    }

    BMP_unload(&bmp);

    read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        return -1;
    }

    palette_t def_plt = {
        .r = 255,
        .g = 255,
        .b = 255
    };

    int resize_res = BMP_resize(&bmp, 60, 60, &def_plt);
    resize_res = FILTER_grayscale(&bmp);
    if (resize_res != 1) {
        printf("BMP_read error! %i\n", read_res);
    }

    const char* resize_larger_path = "default_test_gray_larger.bmp";
    save_res = BMP_save(resize_larger_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
    }

    BMP_unload(&bmp);

    read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        return -1;
    }

    resize_res = BMP_resize(&bmp, 5, 5, NULL);
    resize_res = FILTER_negative(&bmp);
    if (resize_res != 1) {
        printf("BMP_read error! %i\n", read_res);
    }

    const char* resize_smaller_path = "default_test_neg_smaller.bmp";
    save_res = BMP_save(resize_smaller_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
    }

    BMP_unload(&bmp);
    return 1;
}

int __create_filter(const char* path) {
    matrix_t mtrx;
    MTRX_create(&mtrx, 3, 3);
    MTRX_input(&mtrx);
    MTRX_print(&mtrx, "%i ");
    MTRX_save(path, &mtrx);
    MTRX_unload(&mtrx);
    return 1;
}