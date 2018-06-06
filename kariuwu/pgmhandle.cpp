#include "pgmhandle.h"

void clone_pixmap(dimension sx, dimension sy, color ***original, supercolor ***copy)
{
    *copy = (float **) malloc((sy + 2) * sizeof(float *));

    for(dimension py = 0; py < (sy + 2); py++)
    {
        copy[0][py] = (float *) malloc((sx + 2) * sizeof(float));
    }

    for(dimension py = 0; py < sy; py++)
    {
        for(dimension px = 0; px < sx; px++)
        {
            copy[0][py + 1][px + 1] = original[0][py][px];
        }
    }
}

void extend_pixmap(dimension sx, dimension sy, supercolor **array)
{
    dimension px = 0;
    dimension py = 0;

    // Top (0, 1 to n - 2)
    for(py = 0, px = 1; px < (sx - 1); px++)
    {
        array[py][px] = array[py + 1][px];
    }

    // Bottom (n - 1, 1 to n - 2)
    for(py = (sy - 1), px = 1; px < (sx - 1); px++)
    {
        array[py][px] = array[py - 1][px];
    }

    // Left (1 to n - 2, 0)
    for(py = 1, px = 0; py < (sy - 1); py++)
    {
        array[py][px] = array[py][px + 1];
    }

    // Right (1 to n - 2, n - 1)
    for(py = 1, px = (sx - 1); py < (sy - 1); py++)
    {
        array[py][px] = array[py][px - 1];
    }

    // Top-Left Corner (0, 0)
    py = 0;
    px = 0;
    array[py][px] = array[py + 1][px + 1];

    // Top-Right Corner (0, n - 1)
    py = 0;
    px = (sx - 1);
    array[py][px] = array[py + 1][px - 1];

    // Bottom-Left Corner (n - 1, 0)
    py = (sy - 1);
    px = 0;
    array[py][px] = array[py - 1][px + 1];

    // Bottom-Right Corner (n - 1, n - 1)
    py = (sy - 1);
    px = (sx - 1);
    array[py][px] = array[py - 1][px - 1];
}

void interpolate_pixmap(dimension sx, dimension sy, color white, color **original, supercolor superblack, supercolor superwhite, supercolor **copy)
{
    // Original Formula
    //color black = 0;
    //original[py][px] = black + ((copy[py + 1][px + 1] - superblack) * (white - black) ) / (superwhite - superblack);

    for(dimension py = 0; py < sy; py++)
    {
        for(dimension px = 0; px < sx; px++)
        {
            original[py][px] = round(((copy[py + 1][px + 1] - superblack) * white) / (superwhite - superblack));
        }
    }
}

void open_file(pgm *filename, char path[], char *mode)
{
    *filename = fopen(path, mode);
}

void close_file(pgm filename)
{
    fclose(filename);
}

void check_file(pgm filename, header filetype, dimension *sx, dimension *sy, color *white)
{
    char input[8];

    for(int data_string = 1; data_string < 5; data_string++)
    {
        fscanf(filename, "%s", input);

        if(input[0] == '#')
        {
            char trash[1024];
            fgets(trash, 1024, filename);
            --data_string;
        }

        else
        {
            if(data_string == 1)
            {
                strcpy(filetype, input);
            }

            else if(data_string == 2)
            {
                dimension tmp;

                sscanf(input, "%lu", &tmp);
                *sx = tmp;
            }

            else if(data_string == 3)
            {
                dimension tmp;

                sscanf(input, "%lu", &tmp);
                *sy = tmp;
            }

            else if(data_string == 4)
            {
                color tmp;

                sscanf(input, "%hhu", &tmp);
                *white = tmp;
            }
        }
    }
}

void create_file(pgm filename, header filetype, dimension sx, dimension sy, color white, color **array)
{
    if(!strncmp(filetype, "P2", 2))
    {
        fprintf(filename, "P2\n%lu %lu\n%hhu\n", sx, sy, white);

        for(dimension py = 0; py < sy; py++)
        {
            for(dimension px = 0; px < sx; px++)
            {
                fprintf(filename, "%hhu ", array[py][px]);
            }

            fprintf(filename, "\n");
        }
    }

    else
    {
        fprintf(filename, "P5\n%lu %lu\n%hhu\n", sx, sy, white);

        for(dimension py = 0; py < sy; py++)
        {
            for(dimension px = 0; px < sx; px++)
            {
                fprintf(filename, "%c", array[py][px]);
            }
        }
    }
}

void init_pixmap(dimension sx, dimension sy, color ***array)
{
    *array = (color **) malloc(sy * sizeof(color *));

    for(dimension py = 0; py < sy; py++)
    {
        array[0][py] = (color *) malloc(sx * sizeof(color));
    }

    for(dimension py = 0; py < sy; py++)
    {
        for(dimension px = 0; px < sx; px++)
        {
            array[0][py][px] = 0;
        }
    }
}

void import_pixmap(pgm filename, header filetype, dimension sx, dimension sy, color **array)
{
    if(!strncmp(filetype, "P2", 2))
    {
        char input[4];
        color tmp;

        for(dimension py = 0; py < sy; py++)
        {
            for(dimension px = 0; px < sx; px++)
            {
                fscanf(filename, "%s", input);
                sscanf(input, "%hhu", &tmp);
                array[py][px] = tmp;
            }
        }
    }

    else
    {
        for(dimension py = 0; py < sy; py++)
        {
            for(dimension px = 0; px < sx; px++)
            {
                array[py][px] = fgetc(filename);
            }
        }
    }
}

void mask_pixmap(unsigned short int mask, dimension sx, dimension sy, color white, color **array)
{
    float convolution = 0;
    float kernel[3][3];

    supercolor minblack = 0.0f;
    supercolor maxwhite = white;
    supercolor **input = NULL;
    supercolor **output = NULL;

    if(mask == FILTER_EDGE_A)
    {
        kernel[0][0] = 1.0f;
        kernel[0][1] = 0.0f;
        kernel[0][2] = -1.0f;

        kernel[1][0] = 0.0f;
        kernel[1][1] = 0.0f;
        kernel[1][2] = 0.0f;

        kernel[2][0] = -1.0f;
        kernel[2][1] = 0.0f;
        kernel[2][2] = 1.0f;
    }

    else if(mask == FILTER_EDGE_B)
    {
        kernel[0][0] = 0.0f;
        kernel[0][1] = 1.0f;
        kernel[0][2] = 0.0f;

        kernel[1][0] = 1.0f;
        kernel[1][1] = -4.0f;
        kernel[1][2] = 1.0f;

        kernel[2][0] = 0.0f;
        kernel[2][1] = 1.0f;
        kernel[2][2] = 0.0f;
    }

    else if(mask == FILTER_EDGE_C)
    {
        kernel[0][0] = -1.0f;
        kernel[0][1] = -1.0f;
        kernel[0][2] = -1.0f;

        kernel[1][0] = -1.0f;
        kernel[1][1] = 8.0f;
        kernel[1][2] = -1.0f;

        kernel[2][0] = -1.0f;
        kernel[2][1] = -1.0f;
        kernel[2][2] = -1.0f;
    }

    else if(mask == FILTER_SHARPEN)
    {
        kernel[0][0] = 0.0f;
        kernel[0][1] = -1.0f;
        kernel[0][2] = 0.0f;

        kernel[1][0] = -1.0f;
        kernel[1][1] = 5.0f;
        kernel[1][2] = -1.0f;

        kernel[2][0] = 0.0f;
        kernel[2][1] = -1.0f;
        kernel[2][2] = 0.0f;
    }

    clone_pixmap(sx, sy, &array, &input);
    clone_pixmap(sx, sy, &array, &output);
    extend_pixmap((sx + 2), (sy + 2), input);
    extend_pixmap((sx + 2), (sy + 2), output);

    for(dimension py = 1; py < (sy + 1); py++)
    {
        for(dimension px = 1; px < (sx + 1); px++)
        {
            // The current center, (py, px) should always
            // match with the kernel center, (1, 1)
            convolution = 0.0f;

            convolution += (kernel[0][0] * input[py - 1][px - 1]);
            convolution += (kernel[0][1] * input[py - 1][px]);
            convolution += (kernel[0][2] * input[py - 1][px + 1]);

            convolution += (kernel[1][0] * input[py][px - 1]);
            convolution += (kernel[1][1] * input[py][px]);
            convolution += (kernel[1][2] * input[py][px + 1]);

            convolution += (kernel[2][0] * input[py + 1][px - 1]);
            convolution += (kernel[2][1] * input[py + 1][px]);
            convolution += (kernel[2][2] * input[py + 1][px + 1]);

            output[py][px] = convolution;

            if(convolution > maxwhite)
            {
                maxwhite = convolution;
            }

            else if(convolution < minblack)
            {
                minblack = convolution;
            }
        }
    }

    interpolate_pixmap(sx, sy, white, array, minblack, maxwhite, output);
}
