#include "../include/matrix.h"


int MTRX_create(matrix_t* mtrx, int rows, int cols) {
    mtrx->rows = rows;
    mtrx->cols = cols;
    mtrx->body = (double**)malloc(rows * sizeof(double*));
    if (!mtrx->body) return -1;
    for (int i = 0; i < rows; i++) {
        mtrx->body[i] = (double*)malloc(cols * sizeof(double));
        if (!mtrx->body[i]) return -1;
    }

    return 1;
}

int MTRX_create_gaussian_1D(matrix_t* mtrx, double sigma, int axis) {
    int size = 2 * ceil(3 * sigma) + 1;
    int half = size / 2;
    double s2 = 2 * sigma * sigma;
    
    if (axis == 0) {
        if (MTRX_create(mtrx, 1, size) != 1) return -1;
    } 
    else {
        if (MTRX_create(mtrx, size, 1) != 1) return -1;
    }

    double sum = 0.0;
    for (int i = -half; i <= half; i++) {
        double value = exp(-(i * i) / s2);
        if (axis == 0) {
            mtrx->body[0][i + half] = value;
        } else {
            mtrx->body[i + half][0] = value;
        }
        sum += value;
    }

    for (int i = 0; i < size; i++) {
        if (axis == 0) {
            mtrx->body[0][i] /= sum;
        } else {
            mtrx->body[i][0] /= sum;
        }
    }

    return 1;
}

int MTRX_input(matrix_t* mtrx) {
    printf("Input matrix %i %i\n", mtrx->rows, mtrx->cols);
    for (int i = 0; i < mtrx->rows; i++) {
        for (int j = 0; j < mtrx->cols; j++) {
            printf("Element %i %i: ", i, j);
            char input[10] = { 0 };
            scanf("%s", input);
            mtrx->body[i][j] = (double)atoi(input);
        }
    }

    return 1;
}

int MTRX_print(matrix_t* mtrx, char* fmt) {
    for (int i = 0; i < mtrx->rows; i++) {
        for (int j = 0; j < mtrx->cols; j++) printf(fmt, mtrx->body[i][j]);
        printf("\n");
    }

    return 1;
}

int MTRX_read(const char* path, matrix_t* mtrx) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) return -1;

    int offset = 0;
    if (pread(fd, mtrx, 2 * sizeof(int), offset) != 2 * sizeof(int)) {
        close(fd);
        return -2;
    }

    if (MTRX_create(mtrx, mtrx->rows, mtrx->cols) != 1) {
        close(fd);
        return -3;
    }

    offset += 2 * sizeof(int);
    for (int i = 0; i < mtrx->rows; i++) {
        if (pread(fd, mtrx->body[i], mtrx->cols * sizeof(double), offset) != mtrx->cols * sizeof(double)) {
            MTRX_unload(mtrx);
            close(fd);
            return -3;
        }

        offset += mtrx->cols * sizeof(double);
    }

    close(fd);
    return 1;
}

int MTRX_save(const char* path, matrix_t* mtrx) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;

    int offset = 0;
    if (pwrite(fd, mtrx, 2 * sizeof(int), offset) != 2 * sizeof(int)) {
        close(fd);
        return -2;
    }

    offset += 2 * sizeof(int);
    for (int i = 0; i < mtrx->rows; i++) {
        if (pwrite(fd, mtrx->body[i], mtrx->cols * sizeof(double), offset) != mtrx->cols * sizeof(double)) {
            close(fd);
            return -3;
        }

        offset += mtrx->cols * sizeof(double);
    }

    close(fd);
    return 1;
}

int MTRX_unload(matrix_t* mtrx) {
    for (int i = 0; i < mtrx->rows; i++)
        free(mtrx->body[i]);

    free(mtrx->body);
    return 1;
}
