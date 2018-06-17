#include "sobelhandler.hpp"

// Constructors
SobelHandler::SobelHandler(void)
{
    ppKernelGX = (float **) malloc(3 * sizeof(float *));
    for(unsigned short iPY = 0; iPY < 3; iPY++)
    {
        ppKernelGX[iPY] = (float *) malloc(3 * sizeof(float));
        for(unsigned short iPX = 0; iPX < 3; iPX++)
        {
            ppKernelGX[iPY][iPX] = 0.0f;
        }
    }

    ppKernelGX[0][0] = 1.0f;
    ppKernelGX[0][1] = 0.0f;
    ppKernelGX[0][2] = -1.0f;

    ppKernelGX[1][0] = 2.0f;
    ppKernelGX[1][1] = 0.0f;
    ppKernelGX[1][2] = -2.0f;

    ppKernelGX[2][0] = 1.0f;
    ppKernelGX[2][1] = 0.0f;
    ppKernelGX[2][2] = -1.0f;

    ppKernelGY = (float **) malloc(3 * sizeof(float *));
    for(unsigned short iPY = 0; iPY < 3; iPY++)
    {
        ppKernelGY[iPY] = (float *) malloc(3 * sizeof(float));
        for(unsigned short iPX = 0; iPX < 3; iPX++)
        {
            ppKernelGY[iPY][iPX] = 0.0f;
        }
    }

    ppKernelGY[0][0] = 1.0f;
    ppKernelGY[0][1] = 2.0f;
    ppKernelGY[0][2] = 1.0f;

    ppKernelGY[1][0] = 0.0f;
    ppKernelGY[1][1] = 0.0f;
    ppKernelGY[1][2] = 0.0f;

    ppKernelGY[2][0] = -1.0f;
    ppKernelGY[2][1] = -2.0f;
    ppKernelGY[2][2] = -1.0f;
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

// Public functions
float SobelHandler::readPixelKernelGX(const unsigned short cPY, const unsigned short cPX)
{
    return ppKernelGX[cPY][cPX];
}

float SobelHandler::readPixelKernelGY(const unsigned short cPY, const unsigned short cPX)
{
    return ppKernelGY[cPY][cPX];
}

float SobelHandler::calculateGradient(const float cGY, const float cGX)
{
    return sqrt((cGX * cGX) + (cGY * cGY));
}

void SobelHandler::resetValues(void)
{
    if(ppKernelGX != nullptr)
    {
        for(unsigned short iPY = 0; iPY < 3; iPY++)
        {
            free(ppKernelGX[iPY]);
        }

        free(ppKernelGX);
        ppKernelGX = nullptr;
    }

    if(ppKernelGY != nullptr)
    {
        for(unsigned short iPY = 0; iPY < 3; iPY++)
        {
            free(ppKernelGY[iPY]);
        }

        free(ppKernelGY);
        ppKernelGY = nullptr;
    }
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
