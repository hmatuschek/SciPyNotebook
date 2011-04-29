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
#include "preferences.hh"
#include <QSyntaxHighlighter>


CodeCell::CodeCell(QWidget *parent) :
    QTextEdit(parent)
{
    // Disable word-wrap
    this->setWordWrapMode(QTextOption::NoWrap);

    // install syntax highlighter
    this->higlighter = new PythonHighlighter(this);
    this->higlighter->setDocument(this->document());

    // Get default font from preferences:
    Preferences *prefs = Preferences::get();
    this->document()->setDefaultFont(prefs->getFont());

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


void
CodeCell::markLine(size_t line)
{
  QTextEdit::ExtraSelection highlight;

  // Move cursor to position
  highlight.cursor = this->textCursor();
  highlight.cursor.movePosition(QTextCursor::Start);
  highlight.cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line-1);

  // Highlight line
  highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
  highlight.format.setBackground( Qt::yellow );

  QList<QTextEdit::ExtraSelection> extras;
  extras << highlight;
  this->setExtraSelections( extras );
}


void
CodeCell::clearLineMarks()
{
  // just reset with empty extra selections
  QList<QTextEdit::ExtraSelection> extras;
  this->setExtraSelections(extras);
}
