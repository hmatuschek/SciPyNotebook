#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QVBoxLayout>

#include "codecell.h"
#include "resultcell.hh"
#include "cellstatus.hh"



class Cell : public QFrame
{
    Q_OBJECT

protected:
    QBoxLayout *cellbox;
    CodeCell *codecell;
    ResultCell *resultcell;
    CellStatus *cell_status;


public:
    explicit Cell(QWidget *parent=0);

    void evaluate();
    virtual void setFocus();

signals:


public slots:

};

#endif // CELL_H
