#ifndef FILEHANDLER_HPP
    #define FILEHANDLER_HPP

    #include <QString>
    #include <cstdio>

    class FileHandler
    {
        public:
            explicit FileHandler(void);

            void openSrcFile(const QString &, const QString &);
            void openDstFile(const QString &, const QString &);

            void closeSrcFile(void);
            void closeDstFile(void);

            QString readHeaderSrcFile(void);
            unsigned char readPixmapSrcFile(const QString &);

            void writeHeaderDstFile(const QString &, const char);
            void writePixmapDstFile(const QString &, const unsigned char);

        private:
            FILE *pSrcFile;
            FILE *pDstFile;
    };
#endif // FILEHANDLER_HPP
