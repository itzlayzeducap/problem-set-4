#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const float factor = 1.5f;

int main(void) {
    FILE *input = fopen("input.wav", "rb");
    if (input == NULL) {
        printf("failed to read input.wav");
        return 1;
    }

    FILE *output = fopen("output.wav", "wb");
    if (output == NULL) {
        printf("failed to write output.wav");
        fclose(input);
        return 1;
    }
    
    char header[44];

    if (fread(header, 1, 44, input) != 44) {
        printf("failed to read header");

        fclose(input);
        fclose(output);
        return 1;
    }

    if (fwrite(header, 1, 44, output) != 44) {
        printf("failed to write header");

        fclose(input);
        fclose(output);
        return 1;
    }

    int16_t sample;
    while (fread(&sample, sizeof(int16_t), 1, input) == 1) {
        float scaled_sample = sample * factor;
        if (scaled_sample < INT16_MIN) {
            sample = INT16_MIN;
        } else if (scaled_sample > INT16_MAX) {
            sample = INT16_MAX;
        } else {
            sample = (int16_t)scaled_sample;
        }

        if (fwrite(&sample, sizeof(int16_t), 1, output) != 1) {
            printf("failed to write output.wav");

            fclose(input);
            fclose(output);
            return 1;
        }
    }

    fclose(input);
    fclose(output);
}
