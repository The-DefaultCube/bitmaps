#include <stdio.h>

#define DEBUG_ON

#include "save_bitmap.c"
#include "image_converter.c"
#include "mem_allocator.c"

int main() {
    int width = 256, height = 256;
    int ***image = allocate3D(height, width, 3);

    // Generate a gradient image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image[y][x][0] = 255;   // Red
            image[y][x][1] = 0;   // Green
            image[y][x][2] = 0; // Blue
        }
    }
    get_8bpc_image_rgb(image, height, width, 12);
    save_bmp("output36.bmp", width, height, image);

    return 0;
}