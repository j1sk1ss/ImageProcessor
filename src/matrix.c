#include "../include/matrix.h"


int MTRX_create(matrix_t* mtrx, int x, int y) {
    char** matrix_raw = (char**)malloc(x * sizeof(char*));
    if (!matrix_raw) return -1;
    memset(matrix_raw, 0, x * sizeof(char*));

    for (int i = 0; i < x; i++) {
        matrix_raw[i] = (char*)malloc(y);
        if (!matrix_raw[i]) {
            for (int j = i; j >= 0; j--) free(matrix_raw[j]);
            return -2;
        }

        memset(matrix_raw[i], 0, y);
    }

    mtrx->x = x;
    mtrx->y = y;
    mtrx->body = matrix_raw;
    return 1;
}

int MTRX_input(matrix_t* mtrx) {
    printf("Input matrix %i %i\n", mtrx->x, mtrx->y);
    for (int i = 0; i < mtrx->x; i++) {
        for (int j = 0; j < mtrx->y; j++) {
            printf("Element %i %i: ", i, j);
            char input[5] = { 0 };
            scanf("%s", input);
            mtrx->body[i][j] = (char)atoi(input);
        }
    }

    return 1;
}

int MTRX_print(matrix_t* mtrx, char* fmt) {
    for (int i = 0; i < mtrx->x; i++) {
        for (int j = 0; j < mtrx->y; j++) printf(fmt, mtrx->body[i][j]);
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

    if (MTRX_create(mtrx, mtrx->x, mtrx->y) != 1) {
        close(fd);
        return -3;
    }

    offset += 2 * sizeof(int);
    for (int i = 0; i < mtrx->x; i++) {
        if (pread(fd, mtrx->body[i], mtrx->y, offset) != mtrx->y) {
            MTRX_unload(mtrx);
            close(fd);
            return -3;
        }

        offset += mtrx->y;
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
    for (int i = 0; i < mtrx->x; i++) {
        if (pwrite(fd, mtrx->body[i], mtrx->y, offset) != mtrx->y) {
            close(fd);
            return -3;
        }

        offset += mtrx->y;
    }

    close(fd);
    return 1;
}

int MTRX_unload(matrix_t* mtrx) {
    for (int i = 0; i < mtrx->x; i++)
        free(mtrx->body[i]);

    free(mtrx->body);
    return 1;
}
