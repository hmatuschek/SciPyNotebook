#include "cellinputstream.hh"


CellInputStream::CellInputStream(QObject *parent) :
    QObject(parent)
{
}


void
CellInputStream::write(const QString &data)
{
    emit newData(data);
}




