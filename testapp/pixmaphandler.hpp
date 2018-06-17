#ifndef PIXMAPHANDLER_HPP
    #define PIXMAPHANDLER_HPP

    #include <QString>
    #include <cstdlib>
    #include <cmath>
    #include "sobelhandler.hpp"

    class PixmapHandler
    {
        public:
            explicit PixmapHandler(void);

            float getLowBlack(void) const;
            float getHighWhite(void) const;
            unsigned char getColorTreshold(void) const;
            bool getInitializedTreshold(void) const;

            void setLowBlack(const float);
            void setHighWhite(const float);
            void setColorTreshold(const unsigned char);
            void setInitializedTreshold(const bool);

            void allocTruePixmap(const unsigned short, const unsigned short);
            void allocKernelPixmap(const QString &);
            void allocInputPixmap(const unsigned short, const unsigned short);
            void allocOutputPixmap(const unsigned short, const unsigned short);

            void freeTruePixmap(const unsigned short);
            void freeKernelPixmap(void);
            void freeInputPixmap(unsigned short);
            void freeOutputPixmap(const unsigned short);

            unsigned char readPixelTruePixmap(const unsigned short, const unsigned short) const;
            float readPixelKernelPixmap(const unsigned short, const unsigned short) const;
            float readPixelInputPixmap(const unsigned short, const unsigned short) const;
            float readPixelOutputPixmap(const unsigned short, const unsigned short) const;

            void writePixelTruePixmap(const unsigned short, const unsigned short, const unsigned char);
            void writePixelKernelPixmap(const unsigned short, const unsigned short, float);
            void writePixelInputPixmap(const unsigned short, const unsigned short, float);
            void writePixelOutputPixmap(const unsigned short, const unsigned short, float);

            void applyKernelPixmap(unsigned short, unsigned short);
            void interpolatePixmaps(const unsigned short, const unsigned short);
            void invertTruePixmap(const unsigned short, const unsigned short);

        private:
            void extendInputPixmap(const unsigned short, const unsigned short);

            SobelHandler *pSobelHandler;

            unsigned char **ppTruePixmap;
            unsigned char mColorTreshold;

            float **ppKernelPixmap;
            float **ppInputPixmap;
            float **ppOutputPixmap;
            float mLowBlack;
            float mHighWhite;

            bool mInitializedTreshold;
    };
#endif // PIXMAPHANDLER_HPP
