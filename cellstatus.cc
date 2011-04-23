#include "cellstatus.hh"

CellStatus::CellStatus(QWidget *parent) :
    QWidget(parent)
{
    //
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    // set minimum width to 5px:
    this->setMinimumWidth(5);
    this->resize(5,5);

    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::lightGray);
    this->setPalette(pal);
    this->setAutoFillBackground(true);
}


void
CellStatus::setStatusSuccess()
{
    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::green);
    this->setPalette(pal);
}


void
CellStatus::setStatusRunning()
{
    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::yellow);
    this->setPalette(pal);
}


void
CellStatus::setStatusError()
{
    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::red);
    this->setPalette(pal);
}

