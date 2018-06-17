#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// Constructors
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui = new Ui::MainWindow;
    pImageController = new ImageController;

    ui->setupUi(this);

    ui->wPictureInit->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    ui->wPictureTerm->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    ui->wPictureInit->setAlignment(Qt::AlignCenter);
    ui->wPictureTerm->setAlignment(Qt::AlignCenter);
    ui->wPictureInit->setText("");
    ui->wPictureTerm->setText("");

    ui->wProgressBar->reset();
    ui->wProgressBar->setRange(0, 10);
    ui->wProgressBar->setAlignment(Qt::AlignCenter);
    ui->wProgressBar->setTextVisible(false);
    ui->wProgressState->setText("");

    // Update source directory textbox
    QObject::connect
    (
        pImageController,
        SIGNAL(changedSrcPath(QString)),
        this,
        SLOT(updateLineSrcPath(QString))
    );

    // Update destination directory textbox
    QObject::connect
    (
        pImageController,
        SIGNAL(changedDstPath(QString)),
        this,
        SLOT(updateLineDstPath(QString))
    );

    // Update source picture
    QObject::connect
    (
        pImageController,
        SIGNAL(changedSrcPath(QString)),
        this,
        SLOT(updatePictureInit(QString))
    );

    // Update destination picture
    QObject::connect
    (
        pImageController,
        SIGNAL(finishMaskProcedure(QString)),
        this,
        SLOT(updatePictureTerm(QString))
    );

    // Update progress bar
    QObject::connect
    (
        pImageController,
        SIGNAL(onProgressStep(unsigned short)),
        this,
        SLOT(updateProgressStep(unsigned short))
    );

    // Reset destination picture
    QObject::connect
    (
        pImageController,
        SIGNAL(resetPictureTerm(QString)),
        this,
        SLOT(updatePictureTerm(QString))
    );

    // Reset progress bar
    QObject::connect
    (
        this,
        SIGNAL(resetProgress(unsigned short)),
        this,
        SLOT(updateProgressStep(unsigned short))
    );

    // Change "Create" button state
    QObject::connect
    (
        this,
        SIGNAL(changeCreateButton(bool)),
        this,
        SLOT(updateCreateButton(bool))
    );

    // Begin mask procedure
    QObject::connect
    (
        this,
        SIGNAL(triggerMaskProcedure(void)),
        pImageController,
        SLOT(beginMaskProcedure(void))
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

// Public slots
void MainWindow::updateLineSrcPath(QString newSrcPath)
{
    ui->wLineSrcPath->setText(newSrcPath);
}

void MainWindow::updateLineDstPath(QString newDstPath)
{
    ui->wLineDstPath->setText(newDstPath);
}

void MainWindow::updatePictureInit(QString newPath)
{
    ui->wPictureInit->setPixmap
    (
        QPixmap(newPath).scaled
        (
            ui->wPictureInit->width(),
            ui->wPictureInit->height(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );

    emit resetProgress(0);
}

void MainWindow::updatePictureTerm(QString newPath)
{
    if(newPath == nullptr)
    {
        ui->wPictureTerm->clear();
    }

    else
    {
        ui->wPictureTerm->clear();
        ui->wPictureTerm->setPixmap
        (
            QPixmap(newPath).scaled
            (
                ui->wPictureTerm->width(),
                ui->wPictureTerm->height(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );

        emit changeCreateButton(true);
    }
}

void MainWindow::updateProgressStep(unsigned short newStep)
{
    QCoreApplication::processEvents();

    if(newStep == 0)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("");
    }

    else if(newStep == 1)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Opening directories...");
    }

    else if(newStep == 2)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Importing data from source...");
    }

    else if(newStep == 3)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Importing pixmap from source...");
    }

    else if(newStep == 4)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Setting up workplace...");
    }

    else if(newStep == 5)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Applying filter to the image");
    }

    else if(newStep == 6)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Exporting data to destination...");
    }

    else if(newStep == 7)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Exporting pixmap to destination...");
    }

    else if(newStep == 8)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Closing directories...");
    }

    else if(newStep == 9)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Cleaning up workplace...");
    }

    else if(newStep == 10)
    {
        ui->wProgressBar->setValue(newStep);
        ui->wProgressState->setText("Completed");
    }
}

void MainWindow::updateCreateButton(bool newState)
{
    ui->wCreateImage->setEnabled(newState);
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

// Private slots
void MainWindow::on_wActionCredits_triggered(void)
{
    QString boxTitle = QString("PGM Image Filter");
    QString boxMessage = QString("");

    boxMessage.append("Version v1.2.3.\n");
    boxMessage.append("Built on 2018/06/08.\n\n");

    boxMessage.append("v1.1.0: Added console feedback through qInfo().\n");

    boxMessage.append("v1.1.1: Fixed messages showing incorrectly on console.\n");

    boxMessage.append("v1.2.0: Added Sobel Operator to the filter list");
    boxMessage.append(" and added Class SobelHandler to the codebase.\n");

    boxMessage.append("v1.2.1: Renamed all filters to more appropiate names.\n");

    boxMessage.append("v1.2.2: Fixed wrong de-allocation of the pointers created");
    boxMessage.append(" through Class SobelHandler during the ImageController");
    boxMessage.append(" main procedure.\n");

    boxMessage.append("v1.2.3: Fixed a bug where interpolating wouldn't work");
    boxMessage.append(" for Sobel Operator after choosing another filter.\n");

    boxMessage.append("\nCoded and designed by Karina.");

    QMessageBox::about
    (
        this,
        boxTitle,
        boxMessage
    );
}

void MainWindow::on_wLineSrcPath_editingFinished(void)
{
    pImageController->setSrcPath(ui->wLineSrcPath->text());
}

void MainWindow::on_wLineDstPath_editingFinished(void)
{
    pImageController->setDstPath(ui->wLineDstPath->text());
}

void MainWindow::on_wBrowserSrcPath_clicked(void)
{
    pImageController->setSrcPath
    (
        QFileDialog::getOpenFileName
        (
            this,
            "Open Image",
            "C://",
            "Grayscale Image (*.pgm)"
        )
    );
}

void MainWindow::on_wBrowserDstPath_clicked(void)
{   
    pImageController->setDstPath
    (
        QFileDialog::getSaveFileName
        (
            this,
            "Save Image",
            "C://",
            "Grayscale Image (*.pgm)"
        )
    );

    ui->wLineDstPath->setText(pImageController->getDstPath());
}

void MainWindow::on_wMarkOptionWhite_toggled(bool newMaxOutWhite)
{
    pImageController->setMaxOutWhite(newMaxOutWhite);
}

void MainWindow::on_wMarkOptionInvert_toggled(bool newInvertGrayscale)
{
    pImageController->setInvertGrayscale(newInvertGrayscale);
}

void MainWindow::on_wCreateImage_clicked(void)
{
    QString encodingMessage = nullptr;
    QString extraMessage1 = nullptr;
    QString extraMessage2 = nullptr;
    bool proceed = true;

    // Source path validator
    if(proceed)
    {
        if(pImageController->getSrcPath() == nullptr)
        {
            QMessageBox::critical
            (
                this,
                "Missing Source Image",
                "No source image was chosen.\nPlease point a directory before proceeding.",
                "Retry"
            );

            proceed = false;
        }

        else if(!pImageController->verifyPath(pImageController->getSrcPath()))
        {
            QMessageBox::critical
            (
                this,
                "Invalid Source Image",
                "The source directory is invalid.\nPlease point a valid directory.",
                "Retry"
            );

            proceed = false;
        }
    }

    // Destination path validator
    if(proceed)
    {
        if(pImageController->getDstPath() == nullptr)
        {
            QMessageBox::critical
            (
                this,
                "Missing Destination Image",
                "No destination image was chosen.\nPlease point a directory before proceeding.",
                "Retry"
            );

            proceed = false;
        }

        else if(!pImageController->verifyPath(pImageController->getDstPath()))
        {
            QMessageBox::critical
            (
                this,
                "Invalid Destination Image",
                "The destination directory is invalid.\nPlease point a valid directory.",
                "Retry"
            );

            proceed = false;
        }
    }

    // Unique path validator
    if(proceed)
    {
        if(pImageController->getSrcPath() == pImageController->getDstPath())
        {
            QMessageBox::critical
            (
                this,
                "Duplicated Directory",
                "Both source and destination directories are the same.\nPlease point different directories.",
                "Retry"
            );

            proceed = false;
        }
    }

    // Filter selection validator
    if(proceed)
    {
        if(ui->wSelectFilter1->isChecked())
        {
           pImageController->setFilter(ui->wSelectFilter1->text());
        }

        else if(ui->wSelectFilter2->isChecked())
        {
           pImageController->setFilter(ui->wSelectFilter2->text());
        }

        else if(ui->wSelectFilter3->isChecked())
        {
           pImageController->setFilter(ui->wSelectFilter3->text());
        }

        else if(ui->wSelectFilter4->isChecked())
        {
           pImageController->setFilter(ui->wSelectFilter4->text());
        }

        else if(ui->wSelectFilter5->isChecked())
        {
           pImageController->setFilter(ui->wSelectFilter5->text());
        }

        else
        {
            QMessageBox::critical
            (
                this,
                "Missing Filter",
                "No filter was chosen for the output image.\nPlease choose a filter before proceeding.",
                "Retry"
            );

            proceed = false;
        }
    }

    // Encoding selection validator
    if(proceed)
    {
        if(ui->wSelectEncodingP2->isChecked())
        {
           pImageController->setTermEncoding(ui->wSelectEncodingP2->text());
           encodingMessage = ui->wSelectEncodingP2->text();
        }

        else if(ui->wSelectEncodingP5->isChecked())
        {
            pImageController->setTermEncoding(ui->wSelectEncodingP5->text());
            encodingMessage = ui->wSelectEncodingP5->text();
        }

        else
        {
            QMessageBox::critical
            (
                this,
                "Missing Encoding",
                "No encoding was chosen for the output image.\nPlease choose an encoding before proceeding.",
                "Retry"
            );

            proceed = false;
        }
    }

    // Main creation procedure
    if(proceed)
    {
        if(pImageController->getMaxOutWhite())
        {
            extraMessage1 = "White will be maxed out.\n";
        }

        else
        {
            extraMessage1 = "White value will be kept unaltered.\n";
        }

        if(pImageController->getInvertGrayscale())
        {
            extraMessage2 = "Grayscale will be inverted.";
        }

        else
        {
            extraMessage2 = "Grayscale will be kept unaltered.";
        }

        QMessageBox::StandardButton selection = QMessageBox::information
        (              
            this,
            "Confirm",
            QString("%1%2%3%4%5%6%7").arg
            (
                pImageController->getFilter(),
                " filter will be applied.\n",
                "Image will be created using ",
                encodingMessage,
                ".\n\n",
                extraMessage1,
                extraMessage2
            ),
            QMessageBox::Ok,
            QMessageBox::Cancel
        );

        if(selection == QMessageBox::Ok)
        {
            emit changeCreateButton(false);
            emit triggerMaskProcedure();
        }
    }
}
// **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
