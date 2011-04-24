#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QLinkedList>


class HighlightingRule
{
public:
    QRegExp pattern;
    QTextCharFormat format;
};


class PythonHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

protected:
    QLinkedList<HighlightingRule> rules;

    QFont defaultFont;
    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat operatorFormat;


public:
    explicit PythonHighlighter(QTextEdit *parent = 0);


protected:
    virtual void highlightBlock(const QString &text);


signals:
public slots:
};

#endif // PYTHONHIGHLIGHTER_H
