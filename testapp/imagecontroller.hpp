#ifndef IMAGECONTROLLER_HPP
    #define IMAGECONTROLLER_HPP

    #include <QObject>
    #include <QString>
    #include <QDebug>
    #include "pixmaphandler.hpp"
    #include "filehandler.hpp"

    class ImageController : public QObject
    {
        Q_OBJECT

        public:
            explicit ImageController(QObject *parent = nullptr);

            QString getSrcPath() const;
            QString getDstPath() const;
            QString getInitEncoding() const;
            QString getTermEncoding() const;
            QString getFilter() const;
            unsigned short getSX() const;
            unsigned short getSY() const;
            unsigned char getWhite() const;
            bool getMaxOutWhite() const;
            bool getInvertGrayscale() const;

            void setSrcPath(const QString &);
            void setDstPath(const QString &);
            void setInitEncoding(const QString &);
            void setTermEncoding(const QString &);
            void setFilter(const QString &);
            void setSX(const unsigned short);
            void setSY(const unsigned short);
            void setWhite(const unsigned char);
            void setMaxOutWhite(const bool);
            void setInvertGrayscale(const bool);

            bool verifyPath(const QString &);

        private:
            PixmapHandler *pPixmapHandler;
            FileHandler *pFileHandler;

            QString pSrcPath;
            QString pDstPath;
            QString pInitEncoding;
            QString pTermEncoding;
            QString pFilter;
            unsigned short mSX;
            unsigned short mSY;
            unsigned char mWhite;
            bool mMaxOutWhite;
            bool mInvertGrayscale;

        public slots:
            void beginMaskProcedure(void);

        signals:
            void changedSrcPath(QString);
            void changedDstPath(QString);
            void resetPictureTerm(QString);
            void finishMaskProcedure(QString);
            void onProgressStep(unsigned short);
    };
#endif // IMAGECONTROLLER_HPP
