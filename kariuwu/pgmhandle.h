#ifndef PGMHANDLE_H
    #define PGMHANDLE_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>

    #define FILTER_EDGE_A 1
    #define FILTER_EDGE_B 2
    #define FILTER_EDGE_C 3
    #define FILTER_SHARPEN 4

    typedef float supercolor;

    typedef FILE * pgm;
    typedef char header[3];
    typedef unsigned long int dimension;
    typedef unsigned char color;

    void set_kernel();
    void clone_pixmap(dimension, dimension, color ***, supercolor ***);
    void extend_pixmap(dimension, dimension, supercolor **);
    void interpolate_pixmap(dimension, dimension, color, color **, supercolor, supercolor, supercolor **);

    void open_file(pgm *, char [], char *);
    void close_file(pgm);
    void check_file(pgm, header, dimension *, dimension *, color *);
    void create_file(pgm , header, dimension, dimension, color, color **);
    void init_pixmap(dimension, dimension, color ***);
    void import_pixmap(pgm, header, dimension, dimension, color **);
    void mask_pixmap(unsigned short int, dimension, dimension, color, color **);
#endif // PGMHANDLE_H
