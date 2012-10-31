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

#include "notebookview.hh"
#include "preferences.hh"
#include "cellview.hh"

#include <QApplication>
#include <QFile>


NotebookView::NotebookView(QWidget *parent) :
  QFrame(parent), _notebook(0)
{
  // initialize notebook layout and connect signals
  _cell_layout = new QVBoxLayout();
  _cell_layout->setSpacing(0);
  _cell_layout->setContentsMargins(0,0,0,0);
  _cell_layout->setSizeConstraint(QLayout::SetMinimumSize);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  setLayout(_cell_layout);
}


void
NotebookView::setNotebook(Notebook *notebook)
{
  // Disconnect all signals from "old" notebook
  if (0 != _notebook) {
    QObject::disconnect(_notebook, 0, this, 0);
  }

  // Set new notebook and connect to signals:
  _notebook = notebook;
  for (size_t i=0; i<_notebook->numCells(); i++) {
    _cell_layout->addWidget(new CellView(notebook->cell(i)));
  }

  QObject::connect(_notebook, SIGNAL(cellAdded(int,Cell*)), this, SLOT(onCellAdded(int,Cell*)));
  QObject::connect(_notebook, SIGNAL(cellRemoved(int)), this, SLOT(onCellRemoved(int)));
}


Notebook *
NotebookView::notebook() {
  return _notebook;
}


void
NotebookView::onCellAdded(int index, Cell *cell) {
  CellView *view = new CellView(cell);
  _cell_layout->insertWidget(index, view);
  view->setFocus();
}

void
NotebookView::onCellRemoved(int index) {
  QLayoutItem *item = _cell_layout->takeAt(index);
  if (0 != item) { delete item; }
}
