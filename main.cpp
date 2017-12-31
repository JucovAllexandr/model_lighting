#include "widget.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <mainwindow.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mw;
    mw.show();
    //Form f();
    //f.show();
    //w.showFullScreen();

    return a.exec();
}
