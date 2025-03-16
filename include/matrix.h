#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

struct Matrix {
    int x;
    int y;
    char** body;
} typedef matrix_t;


int MTRX_create(matrix_t* mtrx, int x, int y);
int MTRX_create_gaussian(matrix_t* mtrx, double s);
int MTRX_input(matrix_t* mtrx);
int MTRX_print(matrix_t* mtrx, char* fmt);
int MTRX_read(const char* path, matrix_t* mtrx);
int MTRX_save(const char* path, matrix_t* mtrx);
int MTRX_unload(matrix_t* mtrx);

#endif