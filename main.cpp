#include "pgmhandle.h"
#include "strhandle.h"

int main(void)
{
    bool abort_program = false;

    pgm source_image;
    pgm destination_image;
    header encoding;
    dimension width;
    dimension height;
    color scale;
    path source_directory;
    path destination_directory;

    // Open
    show_message(MESSAGE_START);
    get_readpath(source_directory);
    get_writepath(destination_directory);
    source_image = open_file(source_directory, "rb");

    if(source_image == NULL)
    {
        height = 1;
        abort_program = true;
        show_message(ERROR_SRCFILE);
    }


    // Check
    if(!abort_program)
    {
        show_message(PROCESS_READING);
        check_pixmap(source_image, encoding, &width, &height, &scale);

        if(strncmp(encoding, "P2", 2) && strncmp(encoding, "P5", 2))
        {
            abort_program = true;
            show_message(ERROR_HEADER);
        }

    }

    // Import
    color *pixmap[height];

    if(!abort_program)
    {
        show_message(PROCESS_IMPORTING);
        init_pixmap(&width, &height, pixmap);
        import_pixmap(source_image, encoding, &width, &height, &scale, pixmap);
        close_file(source_image);
    }

    // Create
    if(!abort_program)
    {
        show_message(PROCESS_GENERATING);
        destination_image = open_file(destination_directory, "wb");

        if(destination_image == NULL)
        {
            abort_program = true;
            show_message(ERROR_DSTFILE);
        }

        else
        {
            create_file(destination_image, encoding, &width, &height, &scale, pixmap);
            close_file(destination_image);
        }
    }

    // Close
    if(!abort_program)
    {
        show_message(MESSAGE_FINISH);
        return EXIT_SUCCESS;
    }

    else
    {
        show_message(MESSAGE_CRASH);
        return EXIT_FAILURE;
    }
}
