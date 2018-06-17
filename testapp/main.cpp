#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow windowMain;

    qInfo("################################");
    qInfo("Setting window configuration...");
    windowMain.setWindowTitle("PGM Image Filter");
    windowMain.setWindowIcon(QIcon(":icon.png"));
    windowMain.setFixedSize(windowMain.sizeHint());
    windowMain.show();

    qInfo("Ready.");
    qInfo("################################");
    return app.exec();
}
