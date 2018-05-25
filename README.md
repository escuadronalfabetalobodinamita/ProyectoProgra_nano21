#ifndef PGMHANDLE_H
    #define PGMHANDLE_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    typedef FILE * pgm;
    typedef char header[3];
    typedef unsigned long int dimension;
    typedef unsigned char color;

    pgm open_file(char [], char *);
    void close_file(pgm);
    void create_file(pgm , header, dimension *, dimension *, color *, color *[]);
    void init_pixmap(dimension *, dimension *, color *[]);
    void check_pixmap(pgm , header, dimension *, dimension *, color *);
    void import_pixmap(pgm , header, dimension *, dimension *, color *, color *[]);
#endif // PGMHANDLE_H
