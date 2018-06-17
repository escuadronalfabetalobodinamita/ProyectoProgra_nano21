#include "imagecontroller.hpp"

// Constructors
ImageController::ImageController(QObject *parent) : QObject(parent)
{
    pPixmapHandler = new PixmapHandler;
    pFileHandler = new FileHandler;

    pSrcPath = nullptr;
    pDstPath = nullptr;
    pInitEncoding = nullptr;
    pTermEncoding = nullptr;
    pFilter = nullptr;
    mSX = 0;
    mSY = 0;
    mWhite = 0;
    mMaxOutWhite = false;
    mInvertGrayscale = false;
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

// Public functions
QString ImageController::getSrcPath() const
{
    return pSrcPath;
}

QString ImageController::getDstPath() const
{
    return pDstPath;
}

QString ImageController::getInitEncoding() const
{
    return pInitEncoding;
}

QString ImageController::getTermEncoding() const
{
    return pTermEncoding;
}

QString ImageController::getFilter() const
{
    return pFilter;
}

unsigned short ImageController::getSX() const
{
    return mSX;
}

unsigned short ImageController::getSY() const
{
    return mSY;
}

unsigned char ImageController::getWhite() const
{
    return mWhite;
}

bool ImageController::getMaxOutWhite() const
{
    return mMaxOutWhite;
}

bool ImageController::getInvertGrayscale() const
{
    return mInvertGrayscale;
}

void ImageController::setSrcPath(const QString &newSrcPath)
{
    pSrcPath = newSrcPath;
    emit changedSrcPath(newSrcPath);
    emit resetPictureTerm(nullptr);
}

void ImageController::setDstPath(const QString &newDstPath)
{
    pDstPath = newDstPath;
    emit changedDstPath(newDstPath);
}

void ImageController::setInitEncoding(const QString &newInitEncoding)
{
    pInitEncoding = newInitEncoding;
}

void ImageController::setTermEncoding(const QString &newTermEncoding)
{
    if(newTermEncoding == QString("ASCII Mode"))
    {
        pTermEncoding = QString("P2");
    }

    else if(newTermEncoding == QString("Raw Mode"))
    {
        pTermEncoding = QString("P5");
    }
}

void ImageController::setFilter(const QString &newFilter)
{
    pFilter = newFilter;
}

void ImageController::setSX(const unsigned short newSX)
{
    mSX = newSX;
}

void ImageController::setSY(const unsigned short newSY)
{
    mSY = newSY;
}

void ImageController::setWhite(const unsigned char newWhite)
{
    mWhite = newWhite;
}

void ImageController::setMaxOutWhite(const bool newMaxOutWhite)
{
    mMaxOutWhite = newMaxOutWhite;
}

void ImageController::setInvertGrayscale(const bool newInvertGrayscale)
{
    mInvertGrayscale = newInvertGrayscale;
}

bool ImageController::verifyPath(const QString &dubiousPath)
{
    bool confirmation = true;

    if(dubiousPath.length() < 5)
    {
        confirmation = false;
    }

    else if(dubiousPath[dubiousPath.length() - 4] != '.')
    {
        confirmation = false;
    }

    else if(dubiousPath[dubiousPath.length() - 3] != 'p')
    {
        confirmation = false;
    }

    else if(dubiousPath[dubiousPath.length() - 2] != 'g')
    {
        confirmation = false;
    }

    else if(dubiousPath[dubiousPath.length() - 1] != 'm')
    {
        confirmation = false;
    }

    return confirmation;
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

// Public slots
void ImageController::beginMaskProcedure(void)
{
    // STEP 0. Reset GUI
    emit resetPictureTerm(nullptr);
    emit onProgressStep(0);
    qInfo("\n################################");
    qInfo("NOTICE: Beginning mask procedure.");
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 1. Opening files
    emit onProgressStep(1);
    qInfo("\nSTEP 1. Opening files");

    qInfo("Opening source directory...");
    pFileHandler->openSrcFile(getSrcPath(), QString("rb"));

    qInfo("Opening destination directory...");
    pFileHandler->openDstFile(getDstPath(), QString("wb"));
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 2. Importing data
    emit onProgressStep(2);
    qInfo("\nSTEP 2. Importing data");

    qInfo("Importing info from source file...");
    for(unsigned short iString = 1; iString <= 4; iString++)
    {
        if(iString == 1)
        {
            setInitEncoding(pFileHandler->readHeaderSrcFile());
        }

        else if(iString == 2)
        {
            setSX((unsigned short) pFileHandler->readHeaderSrcFile().toInt());
        }

        else if(iString == 3)
        {
            setSY((unsigned short) pFileHandler->readHeaderSrcFile().toInt());
        }

        else if(iString == 4)
        {
            setWhite((unsigned char) pFileHandler->readHeaderSrcFile().toInt());
        }
    }

    qInfo("Imported the following data:");
    qInfo("\tEncoding is %s.", getInitEncoding().toLocal8Bit().data());
    qInfo("\tX is %d.", getSX());
    qInfo("\tY is %d.", getSY());
    qInfo("\tWhite is %d.", getWhite());
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 3. Importing pixmap
    emit onProgressStep(3);
    qInfo("\nSTEP 3. Importing pixmap");

    qInfo("Setting white value...");
    pPixmapHandler->setColorTreshold(getWhite());

    qInfo("Allocating true pixmap...");
    pPixmapHandler->allocTruePixmap(getSY(), getSX());

    qInfo("Importing pixmap from source file...");
    for(unsigned short iPY = 0; iPY < getSY(); iPY++)
    {
        for(unsigned short iPX = 0; iPX < getSX(); iPX++)
        {
            pPixmapHandler->writePixelTruePixmap
            (
                iPY,
                iPX,
                pFileHandler->readPixmapSrcFile(getInitEncoding())
            );
        }
    }
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 4. Setting up workplace
    emit onProgressStep(4);
    qInfo("\nSTEP 4. Setting up workplace");

    qInfo("Allocating kernel pixmap...");
    pPixmapHandler->allocKernelPixmap(getFilter());

    qInfo("Allocating input pixmap...");
    pPixmapHandler->allocInputPixmap(getSY(), getSX());

    qInfo("Allocating output pixmap...");
    pPixmapHandler->allocOutputPixmap(getSY(), getSX());
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 5. Applying filter
    emit onProgressStep(5);
    qInfo("\nSTEP 5. Applying filter");

    qInfo("Applying mask...");
    pPixmapHandler->applyKernelPixmap(getSY(), getSX());

    qInfo("Verifying if white will be maxed out...");
    if(getMaxOutWhite())
    {
        pPixmapHandler->setColorTreshold(255);
        qInfo("\tWhite was maxed out.");
    }

    else
    {
        qInfo("\tWhite was kept untouched.");
    }

    qInfo("Interpolating pixmaps:");
    qInfo("\tColorTreshold is %d", pPixmapHandler->getColorTreshold());
    qInfo("\tLowBlack is %.0f", pPixmapHandler->getLowBlack());
    qInfo("\tHighWhite is %.0f", pPixmapHandler->getHighWhite());
    pPixmapHandler->interpolatePixmaps(getSY(), getSX());
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 6. Exporting data
    emit onProgressStep(6);
    qInfo("\nSTEP 6. Exporting data");

    qInfo("Exporting info to destination file...");
    for(unsigned short iString = 1; iString <= 4; iString++)
    {
        if(iString == 1)
        {
            pFileHandler->writeHeaderDstFile(getTermEncoding(), '\n');
        }

        else if(iString == 2)
        {
            pFileHandler->writeHeaderDstFile(QString::number(getSX()), ' ');
        }

        else if(iString == 3)
        {
            pFileHandler->writeHeaderDstFile(QString::number(getSY()), '\n');
        }

        else if(iString == 4)
        {
            pFileHandler->writeHeaderDstFile(QString::number(getWhite()), '\n');
        }
    }
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 7. Exporting pixmap
    emit onProgressStep(7);
    qInfo("\nSTEP 7. Exporting pixmap");

    qInfo("Verifying if grayscale will be inverted...");
    if(getInvertGrayscale())
    {
        pPixmapHandler->invertTruePixmap(getSY(), getSX());
        qInfo("\tGrayscale was inverted.");
    }

    else
    {
        qInfo("\tGrayscale was kept untouched.");
    }

    qInfo("Exporting pixmap to destination file...");
    for(unsigned short iPY = 0; iPY < getSY(); iPY++)
    {
        for(unsigned short iPX = 0; iPX < getSX(); iPX++)
        {
            pFileHandler->writePixmapDstFile
            (
                getTermEncoding(),
                pPixmapHandler->readPixelTruePixmap(iPY, iPX)
            );
        }
    }
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 8. Closing files
    emit onProgressStep(8);
    qInfo("\nSTEP 8. Closing files");

    qInfo("Closing source directory...");
    pFileHandler->closeSrcFile();

    qInfo("Closing destination directory...");
    pFileHandler->closeDstFile();
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 9. Cleaning workplace
    emit onProgressStep(9);
    qInfo("\nSTEP 9. Cleaning up workplace");

    qInfo("Freeing pixmap...");
    pPixmapHandler->freeTruePixmap(getSY());

    qInfo("Freeing kernel pixmap...");
    pPixmapHandler->freeKernelPixmap();

    qInfo("Freeing input pixmap...");
    pPixmapHandler->freeInputPixmap(getSY());

    qInfo("Freeing output pixmap...");
    pPixmapHandler->freeOutputPixmap(getSY());

    qInfo("Reseting color treshold...");
    pPixmapHandler->setInitializedTreshold(false);
    // **** **** **** **** **** **** **** **** **** ****

    // STEP 10. Completed
    emit onProgressStep(10);
    qInfo("Completed process.");

    qInfo("NOTICE: Finishing mask procedure.");
    qInfo("################################");
    emit finishMaskProcedure(getDstPath());
    // **** **** **** **** **** **** **** **** **** ****
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
