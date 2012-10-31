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

#include "cellview.hh"

#include <QStringListModel>

#include <iostream>


CellView::CellView(Cell *cell, QWidget *parent)
  : QFrame(parent), _cell(cell)
{
  // Init the cell status widget
  _cell_status = new CellStatus();

  // Instantiate empty code cell
  _codecell = new CodeCell(cell);
  _syntax_highlighter = new PythonHighlighter(_codecell);

  // Instantiate result cell
  _resultcell = new ResultCell(cell);

  // Assemble layout
  _cellbox = new QVBoxLayout();
  _cellbox->addWidget(_codecell);
  _cellbox->addWidget(_resultcell);
  _cellbox->setContentsMargins(0,0,0,0);
  _cellbox->setSizeConstraint(QLayout::SetMinimumSize);
  _cellbox->setSpacing(0);

  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget(_cell_status);
  hbox->addLayout(_cellbox);
  hbox->setSpacing(0);
  hbox->setContentsMargins(0,3,0,0);

  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  setLayout(hbox);

  // All keyboard commands go to the code-cell
  setFocusProxy(_codecell);
  _resultcell->setFocusProxy(_codecell);

  QObject::connect(_cell, SIGNAL(evaluationStateChanged(uint,uint)),
                   this, SLOT(onCellEvaluationStateChanged(uint,uint)));
  QObject::connect(_cell, SIGNAL(destroyed()), this, SLOT(onCellDeleted()));
  QObject::connect(_cell, SIGNAL(highlightLine(int)), this, SLOT(onHighlightLine(int)));
}


CellView::~CellView() {
  // Pass...
}


void
CellView::onCellEvaluationStateChanged(unsigned int last_state, unsigned int new_state)
{
  switch (new_state) {
  case Cell::QUEUED:
    _cell_status->setStatusQueued();
    break;

  case Cell::EVALUATING:
    _cell_status->setStatusRunning();
    break;

  case Cell::EVALUATED:
    _cell_status->setStatusSuccess();
    break;

  case Cell::ERROR:
    _cell_status->setStatusError();
    break;

  case Cell::UNEVALUATED:
  default:
    _cell_status->setStatusModified();
    break;
  }
}


void
CellView::onCellDeleted() {
  this->deleteLater();
}


void
CellView::onHighlightLine(int line) {
  _codecell->markLine(line);
}
