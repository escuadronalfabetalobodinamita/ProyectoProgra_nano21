#include "strhandle.h"

void remove_newline(path input)
{
    for(int slot = 0; input[slot] != '\0'; slot++)
    {
        if(input[slot] == '\n')
        {
            input[slot] = '\0';
        }
    }
}


void get_readpath(path input)
{
    printf("Introduzca el directorio de la imagen a convertir, incluyendo el nombre del archivo y su extension .pgm:\n");

    fgets(input, PATH_SIZE, stdin);
    remove_newline(input);
    printf("\n");
}

void get_writepath(path input)
{
    printf("Introduzca el directorio de la imagen a generar, incluyendo el nombre del archivo y su extension .pgm:\n");

    fgets(input, PATH_SIZE, stdin);
    remove_newline(input);
    printf("\n");
}

void show_message(unsigned short int code)
{
    if(code == MESSAGE_START)
    {
        printf("GENERADOR DE IMAGENES .PGM NEGATIVAS\n\n");
        printf("Bienvenido al programa. ");
        printf("Mediante esta aplicacion, puede invertir la escala de grises correspondiente a una imagen\nde extension .pgm. ");
        printf("Todos los nombres de imagenes deberan terminar en extension .pgm,\nde otra manera podran ocurrir errores.\n\n");
    }

    else if(code == MESSAGE_FINISH)
    {
        printf("\nLa imagen ha sido generada correctamente en el directorio especificado. ");
        printf("Gracias por utilizar el programa.\n");
    }

    else if(code == MESSAGE_CRASH)
    {
        printf("\nEl programa termino de manera inesperada. Ninguna imagen fue generada.\n");
    }

    else if(code == PROCESS_READING)
    {
        printf("Leyendo imagen original...\n");
    }

    else if(code == PROCESS_IMPORTING)
    {
        printf("Invirtiendo la escala de grises...\n");
    }

    else if(code == PROCESS_GENERATING)
    {
        printf("Generando imagen final...\n");
    }

    else if(code == ERROR_SRCFILE)
    {
        printf("ERROR: El directorio de la imagen .pgm no existe.\n");
    }

    else if(code == ERROR_DSTFILE)
    {
        printf("ERROR: El directorio de generacion para la imagen .pgm no existe.\n");
    }

    else if(code == ERROR_HEADER)
    {
        printf("ERROR: La imagen seleccionada no es una imagen .pgm valida.\n");
    }

    else
    {
        printf("ERROR: Error interno. Un codigo invalido ha sido recibido. ");
        printf("Si esta viendo este mensaje, por favor reporte este error a los desarrolladores del programa\n");
    }
}
