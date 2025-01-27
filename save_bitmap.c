#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// BMP Header Structure
#pragma pack(push, 1)
typedef struct
{
    uint16_t bfType; // BMP signature ('BM')
    uint32_t bfSize; // File size
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits; // Offset to pixel data
} BMPFileHeader;

typedef struct
{
    uint32_t biSize;         // Header size (40 bytes)
    int32_t biWidth;         // Image width
    int32_t biHeight;        // Image height (negative means top-down)
    uint16_t biPlanes;       // Must be 1
    uint16_t biBitCount;     // Bits per pixel (24 for RGB)
    uint32_t biCompression;  // No compression (0)
    uint32_t biSizeImage;    // Image data size (including padding)
    int32_t biXPelsPerMeter; // Resolution (not important)
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;
#pragma pack(pop)

// Function to save a 3D array as BMP
void save_bmp(const char *filename, int width, int height, int ***image)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        perror("File opening failed");
        return;
    }

    // Calculate row padding (BMP rows must be multiple of 4 bytes)
    int row_padding = (4 - (width * 3) % 4) % 4;
    int row_size = (width * 3) + row_padding;
    int data_size = row_size * height;
    int file_size = 54 + data_size; // 54 = 14 (file header) + 40 (info header)

    // BMP File Header
    BMPFileHeader file_header = {
        .bfType = 0x4D42, // 'BM'
        .bfSize = file_size,
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = 54};

    // BMP Info Header
    BMPInfoHeader info_header = {
        .biSize = 40,
        .biWidth = width,
        .biHeight = -height, // Negative for top-down storage
        .biPlanes = 1,
        .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = data_size,
        .biXPelsPerMeter = 2835, // 72 DPI
        .biYPelsPerMeter = 2835,
        .biClrUsed = 0,
        .biClrImportant = 0};

    // Write headers
    fwrite(&file_header, sizeof(BMPFileHeader), 1, fp);
    fwrite(&info_header, sizeof(BMPInfoHeader), 1, fp);

    // Write pixel data (BMP uses BGR format)
    uint8_t padding[3] = {0, 0, 0}; // Maximum 3 bytes of padding
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uint8_t bgr[3] = {
                (uint8_t)image[y][x][2], // Blue
                (uint8_t)image[y][x][1], // Green
                (uint8_t)image[y][x][0]  // Red
            };
            fwrite(bgr, 1, 3, fp);
        }
        fwrite(padding, 1, row_padding, fp); // Write row padding
    }

    fclose(fp);
#ifdef DEBUG_ON
    printf("BMP saved as %s, size (%d, %d).\n", filename, height, width);
#endif
}