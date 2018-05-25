#include "pgmhandle.h"

pgm open_file(char path[], char *mode)
{
    return fopen(path, mode);
}

void close_file(pgm filename)
{
    fclose(filename);
}

void create_file(pgm filename, header filetype, dimension *columns, dimension *rows, color *white, color *pixels[])
{
    if(!strncmp(filetype, "P2", 2))
    {
        fprintf(filename, "P2\n%lu %lu\n%hhu\n", *columns, *rows, *white);

        for(dimension slot_y = 0; slot_y < *rows; slot_y++)
        {
            for(dimension slot_x = 0; slot_x < *columns; slot_x++)
            {
                fprintf(filename, "%hhu ", pixels[slot_y][slot_x]);
            }

            fprintf(filename, "\n");
        }
    }

    else if(!strncmp(filetype, "P5", 2))
    {
        fprintf(filename, "P5\n%lu %lu\n%hhu\n", *columns, *rows, *white);

        for(dimension slot_y = 0; slot_y < *rows; slot_y++)
        {
            for(dimension slot_x = 0; slot_x < *columns; slot_x++)
            {
                fprintf(filename, "%c", pixels[slot_y][slot_x]);
            }
        }
    }
}

void init_pixmap(dimension *columns, dimension *rows, color *pixels[])
{
    for(dimension slot_y = 0; slot_y < *rows; slot_y++)
    {
        pixels[slot_y] = (color *) malloc(*columns * sizeof(color));
    }

    for(dimension slot_y = 0; slot_y < *rows; slot_y++)
    {
        for(dimension slot_x = 0; slot_x < *columns; slot_x++)
        {
            pixels[slot_y][slot_x] = 0;
        }
    }
}

void check_pixmap(pgm filename, header filetype, dimension *columns, dimension *rows, color *white)
{
    char input[8];

    for(int data_string = 1; data_string < 5; data_string++)
    {
        fscanf(filename, "%s", input);
        //printf("%s", input);

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
                *columns = tmp;
            }

            else if(data_string == 3)
            {
                dimension tmp;

                sscanf(input, "%lu", &tmp);
                *rows = tmp;
            }

            else if(data_string == 4)
            {
                color tmp;

                sscanf(input, "%hhu", &tmp);
                *white = tmp;
            }
        }
    }

    //printf("Header is %s\nDimensions are X:%lu and Y:%lu\nScale of %hhu\n\n", filetype, *columns, *rows, *white);
}

void import_pixmap(pgm filename, header filetype, dimension *columns, dimension *rows, color *white, color *pixels[])
{
    dimension total_size = *columns * *rows;

    if(!strncmp(filetype, "P2", 2))
    {
        char input[3];
        color tmp;

        for(dimension slot_y = 0; slot_y < *rows; slot_y++)
        {
            for(dimension slot_x = 0; slot_x < *columns; slot_x++)
            {
                fscanf(filename, "%s", input);
                sscanf(input, "%hhu", &tmp);
                pixels[slot_y][slot_x] = *white - tmp;
            }
        }
    }

    else if(!strncmp(filetype, "P5", 2))
    {
        for(dimension slot_y = 0; slot_y < *rows; slot_y++)
        {
            for(dimension slot_x = 0; slot_x < *columns; slot_x++)
            {
                pixels[slot_y][slot_x] = *white - fgetc(filename);
            }
        }
    }

    else
    {
        printf("ERROR: El archivo no es de tipo .pgm.\n");
    }
}
