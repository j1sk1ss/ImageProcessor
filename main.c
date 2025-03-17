#include <stdio.h>

#include "include/bitmap.h"
#include "include/filters.h"
#include "include/matrix.h"


static int __crop(const char* path, int nw, int nh, const char* output) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    int resize_res = BMP_resize(&bmp, nw, nh, NULL);
    if (resize_res != 1) {
        printf("BMP_resize error! %i\n", resize_res);
        BMP_unload(&bmp);
        return -2;
    }

    int save_res = BMP_save(output, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -3;
    }

    BMP_unload(&bmp);
    return 1;
}

static int __grayscale(const char* path, const char* output) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    int filter_res = FILTER_grayscale(&bmp);
    if (filter_res != 1) {
        printf("FILTER_grayscale error! %i\n", filter_res);
        BMP_unload(&bmp);
        return -2;
    }

    int save_res = BMP_save(output, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -3;
    }

    BMP_unload(&bmp);
    return 1;
}

static int __negative(const char* path, const char* output) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    int filter_res = FILTER_negative(&bmp);
    if (filter_res != 1) {
        printf("FILTER_negative error! %i\n", filter_res);
        BMP_unload(&bmp);
        return -2;
    }

    int save_res = BMP_save(output, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -3;
    }

    BMP_unload(&bmp);
    return 1;
}

static int __sharpening(const char* path, const char* mtrx_path, const char* output) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    matrix_t mtrx;
    int mtrx_read_res = MTRX_read(mtrx_path, &mtrx);
    if (mtrx_read_res != 1) {
        printf("MTRX_read error! %i\n", mtrx_read_res);
        BMP_unload(&bmp);
        return -2;
    }

    int filter_res = FILTER_apply_matrix(&bmp, &mtrx);
    if (filter_res != 1) {
        printf("FILTER_apply_matrix error! %i\n", filter_res);
        MTRX_unload(&mtrx);
        BMP_unload(&bmp);
        return -3;
    }

    int save_res = BMP_save(output, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -4;
    }

    BMP_unload(&bmp);
    return 1;
}

static int __edge_detection(
    const char* path, const char* mtrx_path, int t, const char* output
) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    matrix_t mtrx;
    int mtrx_read_res = MTRX_read(mtrx_path, &mtrx);
    if (mtrx_read_res != 1) {
        printf("MTRX_read error! %i\n", mtrx_read_res);
        BMP_unload(&bmp);
        return -2;
    }

    int filter_res = FILTER_grayscale(&bmp);
    if (filter_res != 1) {
        printf("FILTER_grayscale error! %i\n", filter_res);
        MTRX_unload(&mtrx);
        BMP_unload(&bmp);
        return -3;
    }

    filter_res = FILTER_apply_matrix(&bmp, &mtrx);
    if (filter_res != 1) {
        printf("FILTER_apply_matrix error! %i\n", filter_res);
        MTRX_unload(&mtrx);
        BMP_unload(&bmp);
        return -4;
    }

    filter_res = FILTER_threshold(&bmp, t);
    if (filter_res != 1) {
        printf("FILTER_threshold error! %i\n", filter_res);
        MTRX_unload(&mtrx);
        BMP_unload(&bmp);
        return -5;
    }

    int save_res = BMP_save(output, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -6;
    }

    BMP_unload(&bmp);
    return 1;
}

static int __gaussian(const char* path, int s, const char* output) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    matrix_t x_kern, y_kern;
    int x_res = MTRX_create_gaussian_1D(&x_kern, s, 0);
    int y_res = MTRX_create_gaussian_1D(&y_kern, s, 1);
    if (x_res != 1 && y_res != 1) {
        printf("MTRX_create_gaussian_1D error! %i %i\n", x_res, y_res);
        MTRX_unload(&x_kern);
        MTRX_unload(&y_kern);
        BMP_unload(&bmp);
        return -2;
    }

    int filter_res = FILTER_apply_axis_matrix(&bmp, &x_kern, &y_kern);
    if (filter_res != 1) {
        printf("FILTER_apply_axis_matrix error! %i\n", filter_res);
        MTRX_unload(&x_kern);
        MTRX_unload(&y_kern);
        BMP_unload(&bmp);
        return -3;
    }

    int save_res = BMP_save(output, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        MTRX_unload(&x_kern);
        MTRX_unload(&y_kern);
        BMP_unload(&bmp);
        return -6;
    }

    MTRX_unload(&x_kern);
    MTRX_unload(&y_kern);
    BMP_unload(&bmp);
    return 1;
}

static int __color(const char* path, int clr, const char* output) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    int filter_res = FILTER_color(&bmp, clr);
    if (filter_res != 1) {
        printf("FILTER_color error! %i\n", filter_res);
        BMP_unload(&bmp);
        return -5;
    }

    int save_res = BMP_save(output, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -6;
    }

    BMP_unload(&bmp);
    return 1;
}

int print_help() {
    printf("Usage: ./image_processor <input file> <output file> [filters]\n");
    printf("Filters:\n");
    printf("  -crop <width> <height>    Crop the image to the specified dimensions.\n");
    printf("  -gs                       Convert the image to grayscale.\n");
    printf("  -neg                      Convert the image to negative.\n");
    printf("  -sharp                    Apply sharpening filter.\n");
    printf("  -edge <threshold>         Apply edge detection with the specified threshold.\n");
    printf("  -blur <sigma>             Apply Gaussian blur with the specified sigma.\n");
    printf("  -color <num>              Filter photo by color.\n");
    printf("\nExample:\n");
    printf("  ./image_processor input.bmp output.bmp -crop 800 600 -gs -blur 0.5\n");
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_help();
        return 0;
    }

    const char* input = argv[1];
    const char* output = argv[2];
    int result_code = 0;
    int init = 0;

    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-crop") == 0) {
            if (i + 2 >= argc) {
                printf("Error: Missing arguments for -crop\n");
                return -1;
            }

            int w = atoi(argv[i + 1]);
            int h = atoi(argv[i + 2]);
            result_code = __crop(init ? output : input, w, h, output);
            i += 2;
            init = 1;
        } 
        else if (strcmp(argv[i], "-gs") == 0) {
            result_code = __grayscale(init ? output : input, output);
            init = 1;
        } 
        else if (strcmp(argv[i], "-neg") == 0) {
            result_code = __negative(init ? output : input, output);
            init = 1;
        } 
        else if (strcmp(argv[i], "-sharp") == 0) {
            result_code = __sharpening(init ? output : input, "sharp.flt", output);
            init = 1;
        } 
        else if (strcmp(argv[i], "-edge") == 0) {
            if (i + 1 >= argc) {
                printf("Error: Missing argument for -edge\n");
                return -1;
            }

            float threshold = atof(argv[i + 1]);
            result_code = __edge_detection(init ? output : input, "edge.flt", threshold * 0xFF, output);
            i += 1;
            init = 1;
        } 
        else if (strcmp(argv[i], "-blur") == 0) {
            if (i + 1 >= argc) {
                printf("Error: Missing argument for -blur\n");
                return -1;
            }
            
            float sigma = atof(argv[i + 1]);
            result_code = __gaussian(init ? output : input, sigma, output);
            i += 1;
            init = 1;
        }
        else if (strcmp(argv[i], "-color") == 0) {
            if (i + 1 >= argc) {
                printf("Error: Missing argument for -color\n");
                return -1;
            }

            int clr = atoi(argv[i + 1]);
            result_code = __color(init ? output : input, clr, output);
            i += 1;
            init = 1;
        }
        else {
            printf("Error: Unknown filter %s\n", argv[i]);
            return -1;
        }

        if (result_code != 1) {
            printf("Error while processing image! [%i]\n", result_code);
            return result_code;
        }
    }

    return 0;
}