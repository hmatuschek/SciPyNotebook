/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "resultcell.hh"
#include <iostream>


ResultCell::ResultCell(QWidget *parent) :
    QTextEdit(parent)
{
    // Please, no line wraps...
    this->setLineWrapMode(QTextEdit::NoWrap);

    // Disable editing...
    this->setReadOnly(true);

    this->_text_size = this->document()->size().toSize();
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QObject::connect(this, SIGNAL(textChanged()),
                     this, SLOT(onTextChanged()));

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, Qt::lightGray);
    this->setPalette(pal);
    this->setAutoFillBackground(true);

    // Disable widget, will be activated once the code runs    
    this->setFrameShape(QFrame::NoFrame);

    // Connect to CellInputStream events:
    QObject::connect(&(this->_stdout_stream), SIGNAL(newData(QString)),
                     this, SLOT(onStdoutText(QString)));
    QObject::connect(&(this->_stderr_stream), SIGNAL(newData(QString)),
                     this, SLOT(onStderrText(QString)));

    // Setup format for stderr and stdout...
    this->stdoutFormat.setFontStyleHint(QFont::TypeWriter, QFont::PreferMatch);
    this->stdoutFormat.setForeground(QColor(0x00, 0x00, 0x00));
    this->stderrFormat.setFontStyleHint(QFont::TypeWriter, QFont::PreferMatch);
    this->stderrFormat.setForeground(QColor(0xa0, 0x00, 0x00));
}


QSize
ResultCell::minimumSizeHint() const
{
    return this->document()->size().toSize();
}


QSize
ResultCell::sizeHint() const
{
    QSize csize(QTextEdit::sizeHint());
    csize.setHeight(this->document()->size().toSize().height());

    return csize;
}


void
ResultCell::onTextChanged()
{
    this->_text_size = this->document()->size().toSize();
    this->updateGeometry();
}


CellInputStream *
ResultCell::getStdoutStream()
{
    return &(this->_stdout_stream);
}


CellInputStream *
ResultCell::getStderrStream()
{
    return &(this->_stderr_stream);
}


void
ResultCell::onStdoutText(const QString &data)
{
    this->textCursor().insertText(data, this->stdoutFormat);
    this->setVisible(true);
}

void
ResultCell::onStderrText(const QString &data)
{
    this->textCursor().insertText(data, this->stderrFormat);
    this->setVisible(true);
}

