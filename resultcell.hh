#ifndef RESULTCELL_HH
#define RESULTCELL_HH

#include <QTextEdit>
#include "cellinputstream.hh"


class ResultCell : public QTextEdit
{
    Q_OBJECT

private:
    QSize _text_size;
    CellInputStream _stdout_stream;
    CellInputStream _stderr_stream;

    QTextCharFormat stdoutFormat;
    QTextCharFormat stderrFormat;

public:
    explicit ResultCell(QWidget *parent = 0);

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;

    CellInputStream *getStdoutStream();
    CellInputStream *getStderrStream();


signals:


public slots:
    void onTextChanged();
    void onStdoutText(const QString &data);
    void onStderrText(const QString &data);
};

#endif // RESULTCELL_HH
