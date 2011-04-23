#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class PythonHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit PythonHighlighter(QObject *parent = 0);

protected:
    virtual void highlightBlock(const QString &text);

signals:

public slots:

};

#endif // PYTHONHIGHLIGHTER_H
