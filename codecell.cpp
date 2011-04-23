#include "codecell.h"
#include <QSyntaxHighlighter>

#include <iostream>


CodeCell::CodeCell(QWidget *parent) :
    QTextEdit(parent)
{
    // Disable word-wrap
    this->setWordWrapMode(QTextOption::NoWrap);

    // install syntax highlighter
    this->higlighter = new PythonHighlighter();
    this->higlighter->setDocument(this->document());

    QFont default_font; default_font.setStyleHint(QFont::TypeWriter);
    this->document()->setDefaultFont(default_font);

    this->_text_size = this->document()->size().toSize();
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QObject::connect(this, SIGNAL(textChanged()),
                     this, SLOT(onTextChanged()));

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setFrameShape(QFrame::NoFrame);
}


CodeCell::~CodeCell()
{
    // destroy the syntax highlighter
    delete this->higlighter;
}



QSize
CodeCell::minimumSizeHint() const
{
    return this->document()->size().toSize();
}


QSize
CodeCell::sizeHint() const
{
    QSize csize(QTextEdit::sizeHint());
    csize.setHeight(this->document()->size().toSize().height());

    return csize;
}


void
CodeCell::onTextChanged()
{
    this->_text_size = this->document()->size().toSize();
    this->updateGeometry();
}
