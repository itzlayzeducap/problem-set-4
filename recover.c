#include <stdio.h>
#include <stdlib.h>

int is_jpeg_header(unsigned char *buffer)
{
    return buffer[0] == 0xff &&
           buffer[1] == 0xd8 &&
           buffer[2] == 0xff &&
           (buffer[3] & 0xf0) == 0xe0; // т.е. от 0xe0 до 0xf0
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("use: ./recover image_file.png\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("failed to read input\n");
        return 1;
    }

    unsigned char buffer[512];
    FILE *output = NULL;

    char filename[8];
    int count = 0;

    while (fread(buffer, 1, 512, input) == 512)
    {
        if (is_jpeg_header(buffer))
        {
            if (output != NULL)
            {
                fclose(output);
            }

            sprintf(filename, "%03d.jpg", count++);
            output = fopen(filename, "wb");
            
            if (output == NULL)
            {
                fclose(input);
                return 1;
            }
        }

        if (output != NULL)
        {
            fwrite(buffer, 1, 512, output);
        }
    }

    if (output != NULL)
    {
        fclose(output);
    }

    fclose(input);
    return 0;
}