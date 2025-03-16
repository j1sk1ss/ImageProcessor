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
Return -10 if args is NULL.
Return 1 if all success.
*/
int FILTER_grayscale(bitmap_t* bmp);

/*
Params:
- bmp - Pointer to bitmap.

Return BMP_get_pixel_24 error codes.
Return BMP_set_pixel_24 * 10 error codes.
Return -10 if args is NULL.
Return 1 if all success.
*/
int FILTER_negative(bitmap_t* bmp);

/*
Params:
- bmp - Pointer to bitmap.
- t - Threshold number.

Return BMP_get_pixel_24 error codes.
Return BMP_set_pixel_24 * 10 error codes.
Return -10 if args is NULL.
Return 1 if all success.
*/
int FILTER_threshold(bitmap_t* bmp, unsigned char t);

/*
Params:
- bmp - Pointer to bitmap.
- mtrx - Pointer to matrix filter.
- aw - Apply weight to sum.

Return -10 if args is NULL.
Return 1 if all success.
*/
int FILTER_apply_matrix(bitmap_t* bmp, matrix_t* mtrx, int aw);

#endif