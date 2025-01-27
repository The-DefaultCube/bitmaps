#include <stdio.h>
#include <stdlib.h>

#define R_CHANNEL 0
#define G_CHANNEL 1
#define B_CHANNEL 2

void get_8bpc_image_rgb(int ***image_rgb, int height, int width, int bit_depth_per_channel)
{
// modify in place
#ifdef DEBUG_ON
    printf("bit-depth: %d.\n", bit_depth_per_channel);
#endif
    int divider = (1 << bit_depth_per_channel) - 1;

    for (int ro = 0; ro < height; ro++)
    {
        for (int col = 0; col < width; col++)
        {
            image_rgb[ro][col][R_CHANNEL] *= 255;
            image_rgb[ro][col][R_CHANNEL] /= divider;

            image_rgb[ro][col][G_CHANNEL] *= 255;
            image_rgb[ro][col][G_CHANNEL] /= divider;

            image_rgb[ro][col][B_CHANNEL] *= 255;
            image_rgb[ro][col][B_CHANNEL] /= divider;
        }
    }
#ifdef DEBUG_ON
    printf("Converted bit-depth: %d to 8-bit image.\n", bit_depth_per_channel);
#endif
    return;
}

void get_8bpc_image_mono(int **image_mono, int height, int width, int bit_depth_per_channel)
{
// modify in place
#ifdef DEBUG_ON
    printf("bit-depth: %d.\n", bit_depth_per_channel);
#endif
    int divider = (1 << bit_depth_per_channel) - 1;

    for (int ro = 0; ro < height; ro++)
    {
        for (int col = 0; col < width; col++)
        {
            image_mono[ro][col] *= 255;
            image_mono[ro][col] /= divider;
        }
    }
#ifdef DEBUG_ON
    printf("Converted bit-depth: %d to 8-bit image.\n", bit_depth_per_channel);
#endif
    return;
}