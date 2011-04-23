#ifndef CELLSTATUS_HH
#define CELLSTATUS_HH

#include <QWidget>

class CellStatus : public QWidget
{
    Q_OBJECT
public:
    explicit CellStatus(QWidget *parent = 0);

    void setStatusSuccess();
    void setStatusError();
    void setStatusRunning();

signals:

public slots:

};

#endif // CELLSTATUS_HH
