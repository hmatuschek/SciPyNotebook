#ifndef CELLINPUTSTREAM_HH
#define CELLINPUTSTREAM_HH

#include <QObject>

class CellInputStream : public QObject
{
    Q_OBJECT
public:
    explicit CellInputStream(QObject *parent = 0);

signals:
    void newData(const QString &data);

public slots:
    void write(const QString &data);
};

#endif // CELLINPUTSTREAM_HH
