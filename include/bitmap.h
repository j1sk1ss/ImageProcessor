#ifndef BITMAP_H
#define BITMAP_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#ifndef _WIN32
    #include <unistd.h>
    #include <libgen.h>
#else
    #include <io.h>
#endif

#define LOAD_PART 256


// https://en.wikipedia.org/wiki/BMP_file_format#/media/File:BMPfileFormat.svg
typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} __attribute__((packed)) bmp_fileheader_t;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} __attribute__((packed)) bmp_infoheader_t;

typedef struct bitmap {
    bmp_fileheader_t fheader;
    bmp_infoheader_t iheader;
    unsigned char* image_data;
} bitmap_t;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} palette_t;


/*
Note: Be sure, that you BMP_unload bmp struct after usage.

Params:
- path - Path to .bmp file.
- bmp - Pointer to allocated struct.

Return -1 if can't open file.
Return -2 if bmp_fileheader_t read error.
Return -3 if bmp_infoheader_t read error.
Return -4 if image_data malloc error.
Return -5 if image_data read error.
Return -10 if provided args is NULL.
Return 1 if all success.
*/
int BMP_read(const char* path, bitmap_t* bmp);

/*
Params:
- path - Path to .bmp file.
- bmp - Pointer to allocated struct.

Return -1 if can't open or create file.
Return -2 if bmp_fileheader_t write error.
Return -3 if bmp_infoheader_t write error.
Return -4 if image_data write error. 
Return -10 if provided args is NULL.
Return 1 if all success.
*/
int BMP_save(const char* path, bitmap_t* bmp);

/*
Params:
- src - Source bitmap.
- dst - Destination bitmap.

Return -1 if malloc error.
Return -10 if provided args is NULL.
Return 1 if all success.
*/
int BMP_copy(bitmap_t* src, bitmap_t* dst);

/*
Params:
- bmp - Pointer to allocated struct.

Return -1 if bitmap is NULL.
Return -10 if provided args is NULL.
Return 1 if all success.
*/
int BMP_unload(bitmap_t* bmp);


/*
Params:
- x - X coordinate in image. (Pixels)
- y - Y coordinate in image. (Pixels)
- bmp - Pointer to allocated struct.
- p - Pallete (Pixel data).

Return -1 if bmp struct not a 24 bit bmp file.
Return -2 if offset larger than image size.
Return -10 if provided args is NULL.
Return 1 if all success.
*/
int BMP_get_pixel_24(int x, int y, bitmap_t* bmp, palette_t* p);

/*
Params:
- x - X coordinate in image. (Pixels)
- y - Y coordinate in image. (Pixels)
- bmp - Pointer to allocated struct.
- p - Pallete (Pixel data).

Return -1 if bmp struct not a 24 bit bmp file.
Return -2 if offset larger than image size.
Return -10 if provided args is NULL.
Return 1 if all success.
*/
int BMP_set_pixel_24(int x, int y, bitmap_t* bmp, palette_t* p);

/*
Params:
- bmp - Pointer to allocated struct.
- nw - New width of image.
- nh - New height of image.
- plt - Pixel data, that will be place at empty space.

Return -1 if bmp is NULL or nw less than 0 or nh less than 0.
Return -2 if was malloc error.
Return -10 if provided args is NULL.
Return 1 if all success.
*/
int BMP_resize(bitmap_t* bmp, int nw, int nh, palette_t* plt);

#endif