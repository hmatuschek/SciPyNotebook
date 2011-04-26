/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#include "codecell.hh"
#include <QSyntaxHighlighter>


CodeCell::CodeCell(QWidget *parent) :
    QTextEdit(parent)
{
    // Disable word-wrap
    this->setWordWrapMode(QTextOption::NoWrap);

    // install syntax highlighter
    this->higlighter = new PythonHighlighter(this);
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
