#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    unsigned char B;
    unsigned char G;    
    unsigned char R;
} BMP;

float grayscale(BMP bmp) {
    return (0.299 * bmp.R + 0.587 * bmp.G + 0.114 * bmp.B);
}

BMP sepia(BMP bmp) {
    float blue = 0.272 * bmp.R + 0.534 * bmp.G + 0.131 * bmp.B;
    float green = 0.349 * bmp.R + 0.686 * bmp.G + 0.168 * bmp.B;
    float red = 0.393 * bmp.R + 0.769 * bmp.G + 0.189 * bmp.B;

    if (blue > 255) {
        blue = 255;
    }
    if (green > 255) {
        green = 255;
    }
    if (red > 255) {
        red = 255;
    }

    return (BMP) { round(blue), round(green), round(red) };
}

void reflect(int height, int width, BMP bmp[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width / 2; j++) {
            BMP temp = bmp[i][j];
            
            bmp[i][j] = bmp[i][width - 1 - j];
            bmp[i][width - 1 - j] = temp;
        }
    }
}

void blur(int height, int width, BMP bmp[height][width]) {
    BMP (*copy)[width] = malloc(height * sizeof(*copy));
    if (copy == NULL) {
        return;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            copy[i][j] = bmp[i][j];
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int blue = 0;
            int green = 0;
            int red = 0;

            int count = 0;

            // считаем пиксели и кол-во цветов пикселей
            for (int row = i - 1; row <= i + 1; row++) {
                for (int column = j - 1; column <= j + 1; column++) {
                    if (row >= 0 && row < height && column >= 0 && column < width) {
                        blue += copy[row][column].B;
                        green += copy[row][column].G;
                        red += copy[row][column].R;

                        count++;
                    }
                }
            }

            // тута сред арифм
            // и round для unsigned char
            bmp[i][j] = (BMP) {
                round((float) blue / count),
                round((float) green / count),
                round((float) red / count)
            };
        }
    }

    free(copy);
}
