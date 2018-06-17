#ifndef MAINWINDOW_HPP
    #define MAINWINDOW_HPP

    #include <QCoreApplication>
    #include <QMainWindow>
    #include <QFileDialog>
    #include <QMessageBox>

    #include "imagecontroller.hpp"

    namespace Ui
    {
        class MainWindow;
    }

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = 0);
            ~MainWindow();

        private:
            Ui::MainWindow *ui;
            ImageController *pImageController;

        public slots:
            void updateLineSrcPath(QString);
            void updateLineDstPath(QString);
            void updatePictureInit(QString);
            void updatePictureTerm(QString);
            void updateProgressStep(unsigned short);
            void updateCreateButton(bool newState);

        private slots:
            void on_wActionCredits_triggered(void);
            void on_wLineSrcPath_editingFinished(void);
            void on_wLineDstPath_editingFinished(void);
            void on_wBrowserSrcPath_clicked(void);
            void on_wBrowserDstPath_clicked(void);
            void on_wMarkOptionWhite_toggled(bool);
            void on_wMarkOptionInvert_toggled(bool);
            void on_wCreateImage_clicked(void);

        signals:
            void resetProgress(unsigned short);
            void changeCreateButton(bool);
            void triggerMaskProcedure(void);
    };
#endif // MAINWINDOW_HPP
