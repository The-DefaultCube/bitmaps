#include <stdlib.h>

int ***allocate3D(int height, int width, int channel){
    int ***array = (int ***)calloc(height, sizeof(int **));
    if (array == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        array[i] = (int **)calloc(width, sizeof(int *));
        if (array[i] == NULL) return NULL;

        for (int j = 0; j < width; j++) {
            array[i][j] = (int *)calloc(channel, sizeof(int)); // Zero-initialized
            if (array[i][j] == NULL) return NULL;
        }
    }
    return array;
}