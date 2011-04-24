#ifndef CODECELL_H
#define CODECELL_H

#include <QPlainTextEdit>
#include <QShortcut>
#include "pythonhighlighter.hh"


class CodeCell : public QTextEdit
{
    Q_OBJECT

protected:
    PythonHighlighter *higlighter;
    QSize _text_size;

public:
    explicit CodeCell(QWidget *parent = 0);
    ~CodeCell();

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;

public slots:
    void onTextChanged();
};

#endif // CODECELL_H
