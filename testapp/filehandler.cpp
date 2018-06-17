#include "filehandler.hpp"

// Constructors
FileHandler::FileHandler(void)
{
    pSrcFile = nullptr;
    pDstFile = nullptr;
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

// Public functions
void FileHandler::openSrcFile(const QString &newPath, const QString &newFileMode)
{
    pSrcFile = fopen(newPath.toLocal8Bit().constData(), newFileMode.toLocal8Bit().constData());
}

void FileHandler::openDstFile(const QString &newPath, const QString &newFileMode)
{
    remove(newPath.toLocal8Bit().constData());
    pDstFile = fopen(newPath.toLocal8Bit().data(), newFileMode.toLocal8Bit().data());
}

void FileHandler::closeSrcFile(void)
{
    fclose(pSrcFile);
}

void FileHandler::closeDstFile(void)
{
    fclose(pDstFile);
}

QString FileHandler::readHeaderSrcFile(void)
{
    char tmpContent[16];

    fscanf(pSrcFile, "%s", tmpContent);
    while(tmpContent[0] == '#')
    {
        char tmpTrash[1024];
        fgets(tmpTrash, 1024, pSrcFile);
        fscanf(pSrcFile, "%s", tmpContent);
    }

    return QString(tmpContent);
}

unsigned char FileHandler::readPixmapSrcFile(const QString &cEncoding)
{
    if(cEncoding == QString("P2"))
    {
        char tmpPixelString[4];
        unsigned char tmpPixelValue;

        fscanf(pSrcFile, "%s", tmpPixelString);
        sscanf(tmpPixelString, "%hhu", &tmpPixelValue);

        return tmpPixelValue;
    }

    else
    {
        return fgetc(pSrcFile);
    }
}

void FileHandler::writeHeaderDstFile(const QString &cString, const char cWhitespace)
{
    fprintf(pDstFile, "%s%c", cString.toLocal8Bit().constData(), cWhitespace);
}

void FileHandler::writePixmapDstFile(const QString &cEncoding, const unsigned char cPixel)
{
    if(cEncoding == QString("P2"))
    {
        char tmpPixelString[4];

        sprintf(tmpPixelString, "%hhu", cPixel);
        fprintf(pDstFile, "%s\n", tmpPixelString);
    }

    else
    {
        fprintf(pDstFile, "%c", cPixel);
    }
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
