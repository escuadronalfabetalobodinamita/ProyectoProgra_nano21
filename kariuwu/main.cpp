#include "pgmhandle.h"
#include "strhandle.h"

int main(void)
{
    // Variables
    bool abort_program = false;

    path source_directory;
    path destination_directory;

    pgm source_image = NULL;
    pgm destination_image = NULL;

    filter mask;

    header encoding;

    dimension width = 0;
    dimension height = 0;

    color scale = 0;
    color **pixmap = NULL;
    // **** **** **** **** **** ****

    // Ask
    show_message(MESSAGE_START);

    get_readpath(source_directory);
    open_file(&source_image, source_directory, "rb");

    if(source_image == NULL)
    {
        abort_program = true;
        show_message(ERROR_SRCFILE);
    }

    else
    {
        close_file(source_image);
    }

    if(!abort_program)
    {
        get_writepath(destination_directory);
        open_file(&destination_image, destination_directory, "wb");

        if(destination_image == NULL)
        {
            abort_program = true;
            show_message(ERROR_DSTFILE);
        }

        else
        {
            close_file(destination_image);

            open_file(&source_image, source_directory, "rb");
            get_filter(&mask);
        }
    }
    // **** **** **** **** **** ****

    // Check
    if(!abort_program)
    {
        show_message(PROCESS_READING);
        check_file(source_image, encoding, &width, &height, &scale);

        if(strncmp(encoding, "P2", 2) && strncmp(encoding, "P5", 2))
        {
            abort_program = true;
            show_message(ERROR_HEADER);
        }
    }
    // **** **** **** **** **** ****

    // Import
    if(!abort_program)
    {
        show_message(PROCESS_IMPORTING);
        init_pixmap(width, height, &pixmap);
        import_pixmap(source_image, encoding, width, height, pixmap);
    }
    // **** **** **** **** **** ****

    // Mask
    if(!abort_program)
    {
        show_message(PROCESS_MASKING);
        mask_pixmap(mask, width, height, scale, pixmap);
    }
    // **** **** **** **** **** ****

    // Create
    if(!abort_program)
    {
        show_message(PROCESS_GENERATING);
        open_file(&destination_image, destination_directory, "wb");
        create_file(destination_image, encoding, width, height, scale, pixmap);
        close_file(destination_image);
    }
    // **** **** **** **** **** ****

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
    // **** **** **** **** **** ****
}
