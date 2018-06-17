#ifndef SOBELHANDLER_HPP
    #define SOBELHANDLER_HPP

    #include <cstdlib>
    #include <cmath>

    class SobelHandler
    {
        public:
            explicit SobelHandler(void);

            float readPixelKernelGX(const unsigned short, const unsigned short);
            float readPixelKernelGY(const unsigned short, const unsigned short);

            float calculateGradient(const float, const float);
            void resetValues(void);

        private:
            float **ppKernelGX;
            float **ppKernelGY;
    };
#endif // SOBELHANDLER_HPP
