#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QObject::connect( &a, SIGNAL(aboutToQuit()), &w, SLOT(AboutToQuitSignaled()) );

    QApplication::setQuitOnLastWindowClosed(false);
    w.show();
    return a.exec();
}
