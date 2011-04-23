#include <QtGui/QApplication>
#include "mainwindow.h"
#include "pythonengine.hh"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int ret_val = a.exec();

    // Shutdown python engine:
    delete PythonEngine::get();

    return ret_val;
}
