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

void get_filter(filter *kernel)
{
    printf("Introduzca el numero correspondiente a la mascara que desea aplicar:\n");
    printf("1. Detector de bordes A\n");
    printf("2. Detector de bordes B\n");
    printf("3. Detector de bordes C\n");
    printf("4. Afilador de imagen\n");

    printf("Filtro: ");
    scanf("%hu", kernel);
    printf("\n");

    if((*kernel < 1) || (*kernel > 4))
    {
        *kernel = 1;
    }
}

void show_message(int code)
{
    if(code == MESSAGE_START)
    {
        printf("GENERADOR DE IMAGENES .PGM SEGMENTADAS\n\n");
        printf("Bienvenido al programa. ");
        printf("Mediante esta aplicacion, puede aplicar una mascara de segmentacion a una imagen\nde extension .pgm. ");
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
        printf("Importando el mapa de pixeles...\n");
    }

    else if(code == PROCESS_MASKING)
    {
        printf("Aplicando la mascara de revelacion...\n");
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
