#include "pixmaphandler.hpp"

// Constructors
PixmapHandler::PixmapHandler(void)
{
    pSobelHandler = nullptr;
    ppTruePixmap = nullptr;
    mColorTreshold = 0;
    ppKernelPixmap = nullptr;
    ppInputPixmap = nullptr;
    ppOutputPixmap = nullptr;
    mLowBlack = 0.0f;
    mHighWhite = 0.0f;
    mInitializedTreshold = false;
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

// Public functions
float PixmapHandler::getLowBlack(void) const
{
    return mLowBlack;
}

float PixmapHandler::getHighWhite(void) const
{
    return mHighWhite;
}

unsigned char PixmapHandler::getColorTreshold(void) const
{
    return mColorTreshold;
}

bool PixmapHandler::getInitializedTreshold(void) const
{
    return mInitializedTreshold;
}

void PixmapHandler::setLowBlack(const float newLowBlack)
{
    mLowBlack = newLowBlack;
}

void PixmapHandler::setHighWhite(const float newHighWhite)
{
    mHighWhite = newHighWhite;
}

void PixmapHandler::setInitializedTreshold(const bool newValue)
{
    mInitializedTreshold = newValue;
}

void PixmapHandler::setColorTreshold(const unsigned char newColorTreshold)
{
    mColorTreshold = newColorTreshold;

    if(getInitializedTreshold() == false)
    {
        setHighWhite((float) mColorTreshold);
        setInitializedTreshold(true);
    }
}

void PixmapHandler::allocTruePixmap(const unsigned short cSY, const unsigned short cSX)
{
    ppTruePixmap = (unsigned char **) malloc(cSY * sizeof(unsigned char *));
    for(unsigned short iPY = 0; iPY < cSY; iPY++)
    {
        ppTruePixmap[iPY] = (unsigned char *) malloc(cSX * sizeof(unsigned char));
        for(unsigned short iPX = 0; iPX < cSX; iPX++)
        {
            ppTruePixmap[iPY][iPX] = 0;
        }
    }
}

void PixmapHandler::allocKernelPixmap(const QString &cFilter)
{
    ppKernelPixmap = (float **) malloc(3 * sizeof(float *));
    for(unsigned short iPY = 0; iPY <  3; iPY++)
    {
        ppKernelPixmap[iPY] = (float *) malloc(3 * sizeof(float));
        for(unsigned short iPX = 0; iPX <  3; iPX++)
        {
            ppKernelPixmap[iPY][iPX] = 0.0f;
        }
    }

    if(pSobelHandler != nullptr)
    {
        pSobelHandler->resetValues();
        delete pSobelHandler;
        pSobelHandler = nullptr;
    }

    if(cFilter == QString("Laplace Operator A"))
    {
        writePixelKernelPixmap(0, 0, 0.0f);
        writePixelKernelPixmap(0, 1, 1.0f);
        writePixelKernelPixmap(0, 2, 0.0f);

        writePixelKernelPixmap(1, 0, 1.0f);
        writePixelKernelPixmap(1, 1, -4.0f);
        writePixelKernelPixmap(1, 2, 1.0f);

        writePixelKernelPixmap(2, 0, 0.0f);
        writePixelKernelPixmap(2, 1, 1.0f);
        writePixelKernelPixmap(2, 2, 0.0f);
    }

    else if(cFilter == QString("Laplace Operator B"))
    {
        writePixelKernelPixmap(0, 0, -1.0f);
        writePixelKernelPixmap(0, 1, -1.0f);
        writePixelKernelPixmap(0, 2, -1.0f);

        writePixelKernelPixmap(1, 0, -1.0f);
        writePixelKernelPixmap(1, 1, 8.0f);
        writePixelKernelPixmap(1, 2, -1.0f);

        writePixelKernelPixmap(2, 0, -1.0f);
        writePixelKernelPixmap(2, 1, -1.0f);
        writePixelKernelPixmap(2, 2, -1.0f);
    }

    else if(cFilter == QString("Sobel Operator"))
    {
        pSobelHandler = new SobelHandler;
    }

    else if(cFilter == QString("Edge Detector"))
    {
        writePixelKernelPixmap(0, 0, 1.0f);
        writePixelKernelPixmap(0, 1, 0.0f);
        writePixelKernelPixmap(0, 2, -1.0f);

        writePixelKernelPixmap(1, 0, 0.0f);
        writePixelKernelPixmap(1, 1, 0.0f);
        writePixelKernelPixmap(1, 2, 0.0f);

        writePixelKernelPixmap(2, 0, -1.0f);
        writePixelKernelPixmap(2, 1, 0.0f);
        writePixelKernelPixmap(2, 2, 1.0f);
    }

    else if(cFilter == QString("Sharpening Mask"))
    {
        writePixelKernelPixmap(0, 0, 0.0f);
        writePixelKernelPixmap(0, 0, -1.0f);
        writePixelKernelPixmap(0, 0, 0.0f);

        writePixelKernelPixmap(1, 0, -1.0f);
        writePixelKernelPixmap(1, 1, 5.0f);
        writePixelKernelPixmap(1, 2, -1.0f);

        writePixelKernelPixmap(2, 0, 0.0f);
        writePixelKernelPixmap(2, 1, -1.0f);
        writePixelKernelPixmap(2, 2, 0.0f);
    }
}

void PixmapHandler::allocInputPixmap(const unsigned short cSY, const unsigned short cSX)
{
    if(ppTruePixmap != nullptr)
    {
        ppInputPixmap = (float **) malloc((cSY + 2) * sizeof(float *));
        for(unsigned short iPY = 0; iPY < (cSY + 2); iPY++)
        {
            ppInputPixmap[iPY] = (float *) malloc((cSX + 2) * sizeof(float));
            for(unsigned short iPX = 0; iPX < (cSX + 2); iPX++)
            {
                writePixelInputPixmap(iPY, iPX, 0.0f);
            }
        }

        for(unsigned short iPY = 1; iPY < (cSY + 1); iPY++)
        {
            for(unsigned short iPX = 1; iPX < (cSX + 1); iPX++)
            {
                writePixelInputPixmap(iPY, iPX, readPixelTruePixmap((iPY - 1), (iPX - 1)));
            }
        }

        extendInputPixmap((cSY + 2), (cSX + 2));
    }
}

void PixmapHandler::allocOutputPixmap(const unsigned short cSY, const unsigned short cSX)
{
    if(ppTruePixmap != nullptr)
    {
        ppOutputPixmap = (float **) malloc(cSY * sizeof(float *));
        for(unsigned short iPY = 0; iPY < cSY; iPY++)
        {
            ppOutputPixmap[iPY] = (float *) malloc(cSX * sizeof(float));
            for(unsigned short iPX = 0; iPX < cSX; iPX++)
            {
                ppOutputPixmap[iPY][iPX] = 0.0f;
            }
        }
    }
}

void PixmapHandler::freeTruePixmap(const unsigned short cSY)
{
    if(ppTruePixmap != nullptr)
    {
        for(unsigned short iPY = 0; iPY < cSY; iPY++)
        {
            free(ppTruePixmap[iPY]);
        }

        free(ppTruePixmap);
        ppTruePixmap = nullptr;
    }
}

void PixmapHandler::freeKernelPixmap(void)
{
    if(ppKernelPixmap != nullptr)
    {
        for(unsigned short iPY = 0; iPY < 3; iPY++)
        {
            free(ppKernelPixmap[iPY]);
        }

        free(ppKernelPixmap);
        ppKernelPixmap = nullptr;
    }
}

void PixmapHandler::freeInputPixmap(unsigned short varSY)
{
    // Fixes offset due to previous pixmap extension
    varSY += 2;

    if(ppInputPixmap != nullptr)
    {
        for(unsigned short iPY = 0; iPY < varSY; iPY++)
        {
            free(ppInputPixmap[iPY]);
        }

        free(ppInputPixmap);
        ppInputPixmap = nullptr;
    }
}

void PixmapHandler::freeOutputPixmap(const unsigned short cSY)
{
    if(ppOutputPixmap != nullptr)
    {
        for(unsigned short iPY = 0; iPY < cSY; iPY++)
        {
            free(ppOutputPixmap[iPY]);
        }

        free(ppOutputPixmap);
        ppOutputPixmap = nullptr;
    }
}

unsigned char PixmapHandler::readPixelTruePixmap(const unsigned short cPY, const unsigned short cPX) const
{
    return ppTruePixmap[cPY][cPX];
}

float PixmapHandler::readPixelKernelPixmap(const unsigned short cPY, const unsigned short cPX) const
{
    return ppKernelPixmap[cPY][cPX];
}

float PixmapHandler::readPixelInputPixmap(const unsigned short cPY, const unsigned short cPX) const
{
    return ppInputPixmap[cPY][cPX];
}

float PixmapHandler::readPixelOutputPixmap(const unsigned short cPY, const unsigned short cPX) const
{
    return ppOutputPixmap[cPY][cPX];
}

void PixmapHandler::writePixelTruePixmap(const unsigned short cPY, const unsigned short cPX, const unsigned char newValue)
{
    ppTruePixmap[cPY][cPX] = newValue;
}

void PixmapHandler::writePixelKernelPixmap(const unsigned short cPY, const unsigned short cPX, const float newValue)
{
    ppKernelPixmap[cPY][cPX] = newValue;
}

void PixmapHandler::writePixelInputPixmap(const unsigned short cPY, const unsigned short cPX, const float newValue)
{
    ppInputPixmap[cPY][cPX] = newValue;
}

void PixmapHandler::writePixelOutputPixmap(const unsigned short cPY, const unsigned short cPX, const float newValue)
{
    ppOutputPixmap[cPY][cPX] = newValue;
}

void PixmapHandler::applyKernelPixmap(unsigned short varSY, unsigned short varSX)
{
    // Fixes offset due to previous pixmap extension
    varSY += 2;
    varSX += 2;

    // Normal procedure
    if(pSobelHandler == nullptr)
    {
        float varConvolution = 0.0f;

        for(unsigned short iPY = 1; iPY < (varSY - 1); iPY++)
        {
            for(unsigned short iPX = 1; iPX < (varSX - 1); iPX++)
            {
                varConvolution = 0.0f;

                varConvolution +=
                (
                    readPixelKernelPixmap(0, 0)
                    *
                    readPixelInputPixmap((iPY - 1), (iPX - 1))
                );
                varConvolution +=
                (
                    readPixelKernelPixmap(0, 1)
                    *
                    readPixelInputPixmap((iPY - 1), iPX)
                );
                varConvolution +=
                (
                    readPixelKernelPixmap(0, 2)
                    *
                    readPixelInputPixmap((iPY - 1), (iPX + 1))
                );

                varConvolution +=
                (
                    readPixelKernelPixmap(1, 0)
                    *
                    readPixelInputPixmap(iPY, (iPX - 1))
                );
                varConvolution +=
                (
                    readPixelKernelPixmap(1, 1)
                    *
                    readPixelInputPixmap(iPY, iPX)
                );
                varConvolution +=
                (
                    readPixelKernelPixmap(1, 2)
                    *
                    readPixelInputPixmap(iPY, (iPX + 1))
                );

                varConvolution +=
                (
                    readPixelKernelPixmap(2, 0)
                    *
                    readPixelInputPixmap((iPY + 1), (iPX - 1))
                );
                varConvolution +=
                (
                    readPixelKernelPixmap(2, 1)
                    *
                    readPixelInputPixmap((iPY + 1), iPX)
                );
                varConvolution +=
                (
                    readPixelKernelPixmap(2, 2)
                    *
                    readPixelInputPixmap((iPY + 1), (iPX + 1))
                );

                writePixelOutputPixmap((iPY - 1), (iPX - 1), varConvolution);

                if(varConvolution > getHighWhite())
                {
                    setHighWhite(varConvolution);
                }

                else if(varConvolution < getLowBlack())
                {
                    setLowBlack(varConvolution);
                }
            }
        }
    }

    // Sobel-specific procedure
    else
    {
        float varGX = 0.0f;
        float varGY = 0.0f;
        float varG = 0.0f;

        for(unsigned short iPY = 1; iPY < (varSY - 1); iPY++)
        {
            for(unsigned short iPX = 1; iPX < (varSX - 1); iPX++)
            {
                varGX = 0.0f;
                varGY = 0.0f;

                // Gradient of X
                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(0, 0)
                    *
                    readPixelInputPixmap((iPY - 1), (iPX - 1))
                );
                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(0, 1)
                    *
                    readPixelInputPixmap((iPY - 1), iPX)
                );
                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(0, 2)
                    *
                    readPixelInputPixmap((iPY - 1), (iPX + 1))
                );

                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(1, 0)
                    *
                    readPixelInputPixmap(iPY, (iPX - 1))
                );
                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(1, 1)
                    *
                    readPixelInputPixmap(iPY, iPX)
                );
                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(1, 2)
                    *
                    readPixelInputPixmap(iPY, (iPX + 1))
                );

                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(2, 0)
                    *
                    readPixelInputPixmap((iPY + 1), (iPX - 1))
                );
                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(2, 1)
                    *
                    readPixelInputPixmap((iPY + 1), iPX)
                );
                varGX +=
                (
                    pSobelHandler->readPixelKernelGX(2, 2)
                    *
                    readPixelInputPixmap((iPY + 1), (iPX + 1))
                );
                // **** **** **** **** **** **** **** ****

                // Gradient of Y
                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(0, 0)
                    *
                    readPixelInputPixmap((iPY - 1), (iPX - 1))
                );
                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(0, 1)
                    *
                    readPixelInputPixmap((iPY - 1), iPX)
                );
                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(0, 2)
                    *
                    readPixelInputPixmap((iPY - 1), (iPX + 1))
                );

                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(1, 0)
                    *
                    readPixelInputPixmap(iPY, (iPX - 1))
                );
                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(1, 1)
                    *
                    readPixelInputPixmap(iPY, iPX)
                );
                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(1, 2)
                    *
                    readPixelInputPixmap(iPY, (iPX + 1))
                );

                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(2, 0)
                    *
                    readPixelInputPixmap((iPY + 1), (iPX - 1))
                );
                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(2, 1)
                    *
                    readPixelInputPixmap((iPY + 1), iPX)
                );
                varGY +=
                (
                    pSobelHandler->readPixelKernelGY(2, 2)
                    *
                    readPixelInputPixmap((iPY + 1), (iPX + 1))
                );
                // **** **** **** **** **** **** **** ****

                // Gradient
                varG = pSobelHandler->calculateGradient(varGY, varGX);
                writePixelOutputPixmap((iPY - 1), (iPX - 1), varG);

                if(varG > getHighWhite())
                {
                    setHighWhite(varG);
                    setLowBlack((float) 0);
                }
                // **** **** **** **** **** **** **** ****
            }
        }
    }
}

void PixmapHandler::interpolatePixmaps(const unsigned short cSY, const unsigned short cSX)
{
    if((ppTruePixmap != nullptr) && (ppOutputPixmap != nullptr))
    {
        for(unsigned short iPY = 0; iPY < cSY; iPY++)
        {
            for(unsigned short iPX = 0; iPX < cSX; iPX++)
            {
                writePixelTruePixmap
                (
                    iPY,
                    iPX,
                    (
                        round
                        (
                            (
                                (
                                    readPixelOutputPixmap(iPY, iPX)
                                    -
                                    getLowBlack()
                                )
                                *
                                getColorTreshold()
                            )
                            /
                            (
                                getHighWhite()
                                -
                                getLowBlack()
                            )
                        )
                    )
                );
            }
        }
    }
}

void PixmapHandler::invertTruePixmap(const unsigned short cSY, const unsigned short cSX)
{
    for(unsigned short iPY = 0; iPY < cSY; iPY++)
    {
        for(unsigned short iPX = 0; iPX < cSX; iPX++)
        {
            writePixelTruePixmap(iPY, iPX, (getColorTreshold() - readPixelTruePixmap(iPY, iPX)));
        }
    }
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

// Private functions
void PixmapHandler::extendInputPixmap(const unsigned short cSY, const unsigned short cSX)
{
    // Top (Y:0, X:1 to X:[n - 2])
    for(unsigned short iPX = 1; iPX < (cSX - 1); iPX++)
    {
        writePixelInputPixmap(0, iPX, readPixelInputPixmap(1, iPX));
    }

    // Bottom (Y:[n - 1], X:1 to X:[n - 2])
    for(unsigned short iPX = 1; iPX < (cSX - 1); iPX++)
    {
        writePixelInputPixmap((cSY - 1), iPX, readPixelInputPixmap((cSY - 2), iPX));
    }

    // Left (Y:1 to Y:[n - 2], X:0)
    for(unsigned short iPY = 1; iPY < (cSY - 1); iPY++)
    {
        writePixelInputPixmap(iPY, 0, readPixelInputPixmap(iPY, 1));
    }

    // Right (Y:1 to Y:[n - 2], X:[n - 1])
    for(unsigned short iPY = 1; iPY < (cSY - 1); iPY++)
    {
        writePixelInputPixmap(iPY, (cSX - 1), readPixelInputPixmap(iPY, (cSX - 2)));
    }

    // Top-Left Corner (Y:0, X:0)
    writePixelInputPixmap(0, 0, readPixelInputPixmap(1, 1));

    // Top-Right Corner (Y:0, X:[n - 1])
    writePixelInputPixmap(0, (cSX - 1), readPixelInputPixmap(1, (cSX - 2)));

    // Bottom-Left Corner (Y:[n - 1], X:0)
    writePixelInputPixmap((cSY - 1), 0, readPixelInputPixmap((cSY - 2), 1));

    // Bottom-Right Corner (Y:[n - 1], X:[n - 1])
    writePixelInputPixmap((cSY - 1), (cSX - 1), readPixelInputPixmap((cSY - 2), (cSX - 2)));
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
