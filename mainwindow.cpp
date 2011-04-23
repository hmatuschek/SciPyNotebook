#include "mainwindow.h"
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Set window title
    this->setWindowTitle("SciPy Studio");
    this->resize(533, 640);

    // Create new notebook widget...
    this->notebook = new Notebook();
    QScrollArea *swin = new QScrollArea();
    swin->setWidget(this->notebook);
    swin->setWidgetResizable(true);

    // layout the stuff...
    this->setCentralWidget(swin);
}



MainWindow::~MainWindow()
{
}
