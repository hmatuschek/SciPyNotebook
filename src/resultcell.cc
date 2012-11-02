/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011, 2012 Hannes Matuschek <hmatuschek AT gmail DOT com>
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


ResultCell::ResultCell(Cell *cell, QWidget *parent) :
    QTextEdit(parent)
{
  setDocument(cell->resultDocument());

  // Please, no line wraps...
  setLineWrapMode(QTextEdit::NoWrap);
  setReadOnly(true);
  setVisible(false);

  _text_size = document()->size().toSize();
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Set background color.
  QPalette pal = this->palette();
  pal.setColor(QPalette::Base, Qt::lightGray);
  setPalette(pal);
  setAutoFillBackground(true);

  // Disable widget, will be activated once the code runs
  setFrameShape(QFrame::NoFrame);

  // Connect events:
  QObject::connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
  QObject::connect(cell, SIGNAL(destroyed()), this, SLOT(onCellDeleted()));
}


ResultCell::~ResultCell() {
}

QSize
ResultCell::minimumSizeHint() const {
  return this->document()->size().toSize();
}


QSize
ResultCell::sizeHint() const {
  QSize csize(QTextEdit::sizeHint());
  csize.setHeight(this->document()->size().toSize().height());
  return csize;
}


void
ResultCell::onTextChanged()
{
  _text_size = document()->size().toSize();
  updateGeometry();
  if (1 >= document()->characterCount()) { setVisible(false); }
  else { setVisible(true); }
}

void
ResultCell::onCellDeleted() {
  setDocument(0);
}
