#include "../include/bitmap.h"


int BMP_read(const char* path, bitmap_t* bmp) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) return -1;

    int offset = 0;
    if (pread(fd, &bmp->fheader, sizeof(bmp_fileheader_t), offset) != sizeof(bmp_fileheader_t)) {
        close(fd);
        return -2;
    }

    offset += sizeof(bmp_fileheader_t);
    if (pread(fd, &bmp->iheader, sizeof(bmp_infoheader_t), offset) != sizeof(bmp_infoheader_t)) {
        close(fd);
        return -3;
    }

    offset += sizeof(bmp_infoheader_t);
    bmp->iheader.biSizeImage = bmp->fheader.bfSize - offset;
    bmp->image_data = (unsigned char*)malloc(bmp->iheader.biSizeImage);
    if (!bmp->image_data) {
        close(fd);
        return -4;
    }

    memset(bmp->image_data, 0, bmp->iheader.biSizeImage);
    if (pread(fd, bmp->image_data, bmp->iheader.biSizeImage, offset) != (ssize_t)bmp->iheader.biSizeImage) {
        free(bmp->image_data);
        close(fd);
        return -5;
    }

    close(fd);
    return 1;
}

int BMP_save(const char* path, bitmap_t* bmp) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;

    int offset = 0;
    if (pwrite(fd, &bmp->fheader, sizeof(bmp_fileheader_t), offset) != sizeof(bmp_fileheader_t)) {
        close(fd);
        return -2;
    }

    offset += sizeof(bmp_fileheader_t);
    if (pwrite(fd, &bmp->iheader, sizeof(bmp_infoheader_t), offset) != sizeof(bmp_infoheader_t)) {
        close(fd);
        return -3;
    }

    offset += sizeof(bmp_infoheader_t);
    if (pwrite(fd, bmp->image_data, bmp->iheader.biSizeImage, offset) != (ssize_t)bmp->iheader.biSizeImage) {
        close(fd);
        return -4;
    }

    close(fd);
    return 1;
}

int BMP_get_pixel_24(int x, int y, bitmap_t* bmp, palette_t* p) {
    if (bmp->iheader.biBitCount != 24) return -1;

    unsigned int bytes_per_pixel = bmp->iheader.biBitCount / 8;
    unsigned int row_stride = ((bmp->iheader.biWidth * bytes_per_pixel + 3) & ~3);
    unsigned int offset = y * row_stride + x * bytes_per_pixel;

    if (offset + 2 >= bmp->iheader.biSizeImage) return -2;
    p->b = bmp->image_data[offset] & 0xFF;
    p->g = bmp->image_data[offset + 1] & 0xFF;
    p->r = bmp->image_data[offset + 2] & 0xFF;

    return 1;
}

int BMP_set_pixel_24(int x, int y, bitmap_t* bmp, palette_t* p) {
    if (bmp->iheader.biBitCount != 24) return -1;
    
    unsigned int bytes_per_pixel = bmp->iheader.biBitCount / 8;
    unsigned int row_stride = ((bmp->iheader.biWidth * bytes_per_pixel + 3) & ~3);
    unsigned int offset = y * row_stride + x * bytes_per_pixel;
    
    if (offset + 2 >= bmp->iheader.biSizeImage) return -2;
    bmp->image_data[offset] = p->b & 0xFF;
    bmp->image_data[offset + 1] = p->g & 0xFF;
    bmp->image_data[offset + 2] = p->r & 0xFF;
    
    return 1;
}

int BMP_resize(bitmap_t* bmp, int nw, int nh, palette_t* plt) {
    if (!bmp || nw <= 0 || nh <= 0) return -1;

    unsigned int bytes_per_pixel = bmp->iheader.biBitCount / 8;
    unsigned int ow = bmp->iheader.biWidth;
    unsigned int oh = bmp->iheader.biHeight;
    unsigned int old_row_stride = ((ow * bytes_per_pixel + 3) & ~3);
    unsigned int new_row_stride = ((nw * bytes_per_pixel + 3) & ~3);

    unsigned char* new_image_data = (unsigned char*)malloc(nw * new_row_stride);
    if (!new_image_data) return -2;

    for (int y = 0; y < nh; y++) {
        for (int x = 0; x < nw; x++) {
            unsigned int new_offset = y * new_row_stride + x * bytes_per_pixel;
            if (x < ow && y < oh) {
                unsigned int old_offset = y * old_row_stride + x * bytes_per_pixel;
                new_image_data[new_offset] = bmp->image_data[old_offset];
                new_image_data[new_offset + 1] = bmp->image_data[old_offset + 1];
                new_image_data[new_offset + 2] = bmp->image_data[old_offset + 2];
            } 
            else {
                if (plt) {
                    new_image_data[new_offset] = plt->b & 0xFF;
                    new_image_data[new_offset + 1] = plt->g & 0xFF;
                    new_image_data[new_offset + 2] = plt->r & 0xFF;
                }
            }
        }
    }

    free(bmp->image_data);

    bmp->iheader.biWidth = nw;
    bmp->iheader.biHeight = nh;
    bmp->iheader.biSizeImage = nh * new_row_stride;
    bmp->image_data = new_image_data;

    return 1;
}

int BMP_copy(bitmap_t* src, bitmap_t* dst) {
    memcpy(&dst->fheader, &src->fheader, sizeof(bmp_fileheader_t));
    memcpy(&dst->iheader, &src->iheader, sizeof(bmp_infoheader_t));
    dst->image_data = (unsigned char*)malloc(src->iheader.biSizeImage);
    if (!dst->image_data) return -1;

    memcpy(dst->image_data, src->image_data, src->iheader.biSizeImage);
    return 1;
}

int BMP_unload(bitmap_t* bmp) {
    if (!bmp) return -1;
    if (bmp->image_data) free(bmp->image_data);
    return 1;
}
