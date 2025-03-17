#ifndef _H_MATRIX
#define _H_MATRIX

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

struct {
    int rows, cols;
    double** body;
} typedef matrix_t;


int MTRX_create(matrix_t* mtrx, int rows, int cols);

/*
- axis:
    - 0 - Horizontal
    - 1 - Vertical
*/
int MTRX_create_gaussian_1D(matrix_t* mtrx, double sigma, int axis);
int MTRX_input(matrix_t* mtrx);
int MTRX_print(matrix_t* mtrx, char* fmt);
int MTRX_read(const char* path, matrix_t* mtrx);
int MTRX_save(const char* path, matrix_t* mtrx);
int MTRX_unload(matrix_t* mtrx);

#endif