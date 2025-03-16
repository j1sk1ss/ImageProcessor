#ifndef _H_FILTERS
#define _H_FILTERS

#include "bitmap.h"
#include "matrix.h"
#include "common.h"

#define IGNORE  -128

/*
Params:
- bmp - Pointer to bitmap.

Return BMP_get_pixel_24 error codes.
Return BMP_set_pixel_24 * 10 error codes.
Return 1 if all success.
*/
int FILTER_grayscale(bitmap_t* bmp);

/*
Params:
- bmp - Pointer to bitmap.

Return BMP_get_pixel_24 error codes.
Return BMP_set_pixel_24 * 10 error codes.
Return 1 if all success.
*/
int FILTER_negative(bitmap_t* bmp);

int FILTER_threshold(bitmap_t* bmp, int t);

int FILTER_apply_matrix(bitmap_t* bmp, matrix_t* mtrx);

#endif