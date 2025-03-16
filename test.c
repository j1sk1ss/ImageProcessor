#include <stdio.h>
#include "include/bitmap.h"
#include "include/filters.h"
#include "include/matrix.h"


int __create_filter(const char* path) {
    matrix_t mtrx;
    MTRX_create(&mtrx, 3, 3);
    MTRX_input(&mtrx);
    MTRX_print(&mtrx, "%i ");
    MTRX_save(path, &mtrx);
    MTRX_unload(&mtrx);
    return 1;
}

/*
#### Crop (-crop width height)
Обрезает изображение до заданных ширины и высоты. 
Используется верхняя левая часть изображения.

Если запрошенные ширина или высота превышают размеры 
исходного изображения, выдается доступная часть 
изображения.
*/
static int __crop_test(const char* path, int nw, int nh) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    int resize_res = BMP_resize(&bmp, nw, nh, NULL);
    if (resize_res != 1) {
        printf("BMP_resize error! %i\n", read_res);
        BMP_unload(&bmp);
        return -2;
    }

    const char* save_path = "__crop_test.bmp";
    int save_res = BMP_save(save_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -3;
    }

    BMP_unload(&bmp);
    return 1;
}

/*
#### Grayscale (-gs)
Преобразует изображение в оттенки серого по формуле
*/
static int __grayscale_test(const char* path) {
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

    const char* save_path = "__grayscale_test.bmp";
    int save_res = BMP_save(save_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -3;
    }

    BMP_unload(&bmp);
    return 1;
}

/*
#### Negative (-neg)
Преобразует изображение в негатив по формуле
*/
static int __negative_test(const char* path) {
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

    const char* save_path = "__negative_test.bmp";
    int save_res = BMP_save(save_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -3;
    }

    BMP_unload(&bmp);
    return 1;
}

/*
#### Sharpening (-sharp)
Повышение резкости. Достигается применением матрицы
*/
static int __sharpening_test(const char* path, const char* mtrx_path) {
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

    int filter_res = FILTER_apply_matrix(&bmp, &mtrx, 0);
    if (filter_res != 1) {
        printf("FILTER_apply_matrix error! %i\n", filter_res);
        MTRX_unload(&mtrx);
        BMP_unload(&bmp);
        return -3;
    }

    const char* save_path = "__sharpening_test.bmp";
    int save_res = BMP_save(save_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -4;
    }

    BMP_unload(&bmp);
    return 1;
}

/*
#### Edge Detection (-edge threshold)
Выделение границ. Изображение переводится 
в оттенки серого и применяется матрица
*/
int __edge_detection_test(
    const char* path, const char* mtrx_path, int t
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

    filter_res = FILTER_apply_matrix(&bmp, &mtrx, 0);
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

    const char* save_path = "__edge_detection_test.bmp";
    int save_res = BMP_save(save_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -6;
    }

    BMP_unload(&bmp);
    return 1;
}

/*
#### Gaussian Blur (-blur sigma)
[Гауссово размытие](https://ru.wikipedia.org/wiki/Размытие_по_Гауссу),
параметр – сигма.
Значение каждого из цветов пикселя `C[x0][y0]` определяется формулой
Существуют различные варианты релализации и оптимизации 
вычисления этого фильтра, описание есть 
[в Википедии](https://ru.wikipedia.org/wiki/Размытие_по_Гауссу).
*/
int __gaussian_test(const char* path, int s) {
    bitmap_t bmp;
    int read_res = BMP_read(path, &bmp);
    if (read_res != 1) {
        printf("BMP_read error! %i\n", read_res);
        BMP_unload(&bmp);
        return -1;
    }

    matrix_t kern;
    int matrix_res = MTRX_create_gaussian(&kern, s);
    if (matrix_res != 1) {
        printf("MTRX_create_gaussian error! %i\n", matrix_res);
        BMP_unload(&bmp);
        return -2;
    }

    int filter_res = FILTER_apply_matrix(&bmp, &kern, 1);
    if (filter_res != 1) {
        printf("FILTER_threshold error! %i\n", filter_res);
        MTRX_unload(&kern);
        BMP_unload(&bmp);
        return -3;
    }

    const char* save_path = "__gaussian_test.bmp";
    int save_res = BMP_save(save_path, &bmp);
    if (save_res != 1) {
        printf("BMP_save error! %i\n", save_res);
        BMP_unload(&bmp);
        return -6;
    }

    return 1;
}

int main(int argc, char* argv[]) {
    __crop_test("lenna.bmp", 200, 200);
    __grayscale_test("lenna.bmp");
    __negative_test("lenna.bmp");
    __sharpening_test("lenna.bmp", "sharp.flt");
    __edge_detection_test("lenna.bmp", "edge.flt", 50);
    __gaussian_test("lenna.bmp", 4);
    return 0;
}