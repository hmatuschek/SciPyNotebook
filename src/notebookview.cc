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
  QFrame(parent), _notebook(0), _cells(), _active_cell(0)
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

  // Disconnect from all cells:
  for (QList<CellView *>::iterator cell_view=_cells.begin(); cell_view!=_cells.end(); cell_view++) {
    // Disconnect cell from this view:
    QObject::disconnect((*cell_view)->cell(), 0, this, 0);
    // Remove cell view from layout:
    _cell_layout->removeWidget(*cell_view);
  }

  // Clear list of cells:
  _cells.clear();

  // Set new notebook and connect to signals:
  _notebook = notebook;
  for (size_t i=0; i<_notebook->numCells(); i++) {
    _cells.append(new CellView(notebook->cell(i)));
    QObject::connect(
          _cells.back(), SIGNAL(cellActivated(CellView*)), this, SLOT(onCellActivated(CellView*)));
    QObject::connect(
          _cells.back(), SIGNAL(cellDeactivated(CellView*)), this, SLOT(onCellDeactivated(CellView*)));
    _cell_layout->addWidget(_cells.back());
  }

  QObject::connect(_notebook, SIGNAL(cellAdded(int, Cell*)), this, SLOT(onCellAdded(int,Cell*)));
  QObject::connect(_notebook, SIGNAL(cellRemoved(int)), this, SLOT(onCellRemoved(int)));
}


Notebook *
NotebookView::notebook() {
  return _notebook;
}


void
NotebookView::onCellAdded(int index, Cell *cell) {
  CellView *view = new CellView(cell);
  _cells.insert(index, view);
  _cell_layout->insertWidget(index, view);
  QObject::connect(view, SIGNAL(cellActivated(CellView*)), this, SLOT(onCellActivated(CellView*)));
  QObject::connect(view, SIGNAL(cellDeactivated(CellView*)), this, SLOT(onCellDeactivated(CellView*)));
  view->setFocus();
}

void
NotebookView::onCellRemoved(int index) {
  CellView *view = _cells.at(index);
  QObject::disconnect(view, 0, this, 0);
  _cells.removeAt(index);
  QLayoutItem *item = _cell_layout->takeAt(index);
  if (0 != item) { delete item; }
}


void
NotebookView::onCellActivated(CellView *cell)
{
  if (! _cells.contains(cell)) { return; }
  _active_cell = cell;
}

void
NotebookView::onCellDeactivated(CellView *cell)
{
  _active_cell = 0;
}

void
NotebookView::undo() {
  if (0 == _active_cell) { return; }
  _active_cell->undo();
}

void
NotebookView::redo() {
  if (0 == _active_cell) { return; }
  _active_cell->redo();
}

void
NotebookView::copy() {
  if (0 == _active_cell) { return; }
  _active_cell->copy();
}

void
NotebookView::cut() {
  if (0 == _active_cell) { return; }
  _active_cell->cut();
}

void
NotebookView::paste() {
  if (0 == _active_cell) { return; }
  _active_cell->paste();
}


void
NotebookView::newCell() {
  if (0 == _active_cell) {
    // Create cell at end:
    _notebook->newCell(_cells.size());
  } else {
    int index = _cells.indexOf(_active_cell);
    _notebook->newCell(index+1);
  }
}

void
NotebookView::deleteCell() {
  if (0 == _active_cell) { return; }

  int index = _cells.indexOf(_active_cell);
  if (0 > index) { return; }
  _notebook->deleteCell(index);
}

void
NotebookView::splitCell() {
  if (0 == _active_cell) { return; }
  int index = _cells.indexOf(_active_cell);
  if (0 > index) { return; }
  int off = _active_cell->currentLineOffset();
  _notebook->splitCell(index, off);
}

void
NotebookView::joinCell() {
  if (0 == _active_cell) { return; }
  int index = _cells.indexOf(_active_cell);
  if (0 > index) { return; }
  _notebook->joinCell(index);
}

void
NotebookView::evalCell() {
  if (0 == _active_cell) { return; }
  int index = _cells.indexOf(_active_cell);
  if (0 > index) { return; }
  _notebook->evalCell(index);
}
