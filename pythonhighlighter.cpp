#include "pythonhighlighter.h"
#include <iostream>


PythonHighlighter::PythonHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
}


void
PythonHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat defaultFormat;
    QFont mono("Monospace");
    mono.setStyleHint(QFont::TypeWriter);
    defaultFormat.setFont(mono);

    this->setFormat(0, text.size(), defaultFormat);
}
