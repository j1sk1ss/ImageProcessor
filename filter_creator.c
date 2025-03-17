#include <stdio.h>
#include "include/bitmap.h"
#include "include/filters.h"
#include "include/matrix.h"


int __create_filter(const char* path) {
    matrix_t mtrx;
    MTRX_create(&mtrx, 3, 3);
    MTRX_input(&mtrx);
    MTRX_print(&mtrx, "%f ");
    MTRX_save(path, &mtrx);
    MTRX_unload(&mtrx);
    return 1;
}

int main(int argc, char* argv[]) {
    __create_filter("edge.flt");
    return 0;
}