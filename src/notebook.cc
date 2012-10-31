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

#include "notebook.hh"
#include "cell.hh"
#include "preferences.hh"
#include "pythonengine.hh"
#include "application.hh"

#include <QFile>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>

#include <iostream>


void __remove_whitespaces_from_end(QByteArray &array) {
  if (0 == array.size()) { return; }
  while (isspace(array[array.size()-1])) {
    array.remove(array.size()-1, 1);
  }
}


Notebook::Notebook(QObject *parent)
  : QObject(parent), _filepath(""), _python_context(0)
{
  // Create new, empty context.
  _python_context = new PythonContext(this);

  // Register notebook at application:
  Application *application = static_cast<Application *>(QApplication::instance());
  application->addNotebook(this);

  // Create new cell with default preamble
  Cell *cell = new Cell(this);
  cell->setCode(Preferences::get()->preamble());
  cell->setModified(true);
  _cells.append(cell);
  QObject::connect(cell, SIGNAL(cellActivated(Cell*)), this, SLOT(onCellActivated(Cell*)));
  QObject::connect(cell, SIGNAL(cellDeactivated(Cell*)), this, SLOT(onCellDeactivated(Cell*)));
  QObject::connect(cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));

  // Create actions
  _createActions();
}


Notebook::Notebook(const QString &path, QObject *parent)
  : QObject(parent), _filepath(path), _python_context(0)
{
  // Create new empty context
  _python_context = new PythonContext(this);

  // Register notebook at application:
  Application *application = static_cast<Application *>(QApplication::instance());
  application->addNotebook(this);

  // Read from file:
  QFile file(_filepath);
  if (! file.open(QIODevice::ReadOnly | QIODevice::Text)) { return; }

  QRegExp cellSepExpr("^#\\s+-\\*-\\s+snip\\s+-\\*-\\s*$");
  QList<QByteArray> cells;
  cells.append(QByteArray());

  while(! file.atEnd()) {
    // Read line:
    QByteArray line = file.readLine();
    // check if line is cell separator
    if (cellSepExpr.indexIn(line) != -1) {
      // add an new cell to list
      cells.append(QByteArray());
    } else {
      // append line to current cell
      cells.back().append(line);
    }
  }

  // Create cells from code:
  foreach (QByteArray code, cells) {
    Cell *cell = new Cell(this);
    __remove_whitespaces_from_end(code);
    cell->setCode(code);
    cell->setModified(false);
    _cells.append(cell);
    QObject::connect(cell, SIGNAL(cellActivated(Cell*)), this, SLOT(onCellActivated(Cell*)));
    QObject::connect(cell, SIGNAL(cellDeactivated(Cell*)), this, SLOT(onCellDeactivated(Cell*)));
    QObject::connect(cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));
  }

  // Create actions
  _createActions();
}


void
Notebook::_createActions()
{
  _saveAction = new QAction(tr("Save"), this);
  _saveAction->setShortcut(QKeySequence::Save);

  _saveAsAction = new QAction(tr("Save as..."), this);
  _saveAsAction->setShortcut(QKeySequence::SaveAs);

  _printAction = new QAction(tr("Print..."), this);
  _printAction->setShortcut(QKeySequence::Print);
  _printAction->setVisible(false);

  _undoAction = new QAction(tr("Undo"), this);
  _undoAction->setShortcut(QKeySequence::Undo);
  _redoAction = new QAction(tr("Redo"), this);
  _redoAction->setShortcut(QKeySequence::Redo);
  _copyAction = new QAction(tr("Copy"), this);
  _copyAction->setShortcut(QKeySequence::Copy);
  _cutAction = new QAction(tr("Cut"), this);
  _cutAction->setShortcut(QKeySequence::Cut);
  _pasteAction = new QAction(tr("Paste"), this);
  _pasteAction->setShortcut(QKeySequence::Paste);

  _newCellAction = new QAction(tr("New Cell"), this);
  _newCellAction->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_N);

  _deleteCellAction = new QAction(tr("Delete Cell"), this);
  _deleteCellAction->setShortcut(Qt::CTRL + Qt::Key_Backspace);

  _splitCellAction = new QAction(tr("Split Cell"), this);
  _splitCellAction->setShortcut(Qt::CTRL + Qt::Key_Space);

  _joinCellsAction = new QAction(tr("Join Cells"), this);
  _joinCellsAction->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_Space);

  _evalCellAction = new QAction(tr("Evaluate Cell"), this);
  _evalCellAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));

  _evalAllCellsAction = new QAction(tr("Evaluate all Cells"), this);
  _evalAllCellsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Return));

  QObject::connect(_saveAction, SIGNAL(triggered()), this, SLOT(onSave()));
  QObject::connect(_saveAsAction, SIGNAL(triggered()), this, SLOT(onSaveAs()));
  QObject::connect(_undoAction, SIGNAL(triggered()), this, SLOT(onUndo()));
  QObject::connect(_redoAction, SIGNAL(triggered()), this, SLOT(onRedo()));
  QObject::connect(_newCellAction, SIGNAL(triggered()), this, SLOT(onNewCell()));
  QObject::connect(_deleteCellAction, SIGNAL(triggered()), this, SLOT(onDeleteCell()));
  QObject::connect(_joinCellsAction, SIGNAL(triggered()), this, SLOT(onJoinCell()));
  QObject::connect(_splitCellAction, SIGNAL(triggered()), this, SLOT(onSplitCell()));
  QObject::connect(_evalCellAction, SIGNAL(triggered()), this, SLOT(onEvalCell()));
  QObject::connect(_evalAllCellsAction, SIGNAL(triggered()), this, SLOT(onEvalAllCells()));
}


PythonContext *
Notebook::context() {
  return _python_context;
}


bool
Notebook::hasFileName() const {
  return 0 != _filepath.size();
}

void
Notebook::setFileName(const QString &filename) {
  _filepath = filename;
}

const QString &
Notebook::fileName() const {
  return _filepath;
}

size_t
Notebook::numCells() const {
  return _cells.size();
}

Cell *
Notebook::cell(size_t i) {
  return _cells.at(i);
}


bool
Notebook::isModified() const {
  for (QList<Cell *>::const_iterator cell=_cells.constBegin(); cell!=_cells.constEnd(); cell++) {
    if ((*cell)->isModified()) { return true; }
  }
  return false;
}


void
Notebook::onCellActivated(Cell *cell) {
  if (! _cells.contains(cell)) {
    _active_cell = 0;
    _evalCellAction->setEnabled(false);
    return;
  }

  _active_cell = cell;
  _evalCellAction->setEnabled(true);
}

void
Notebook::onCellDeactivated(Cell *cell) {
  _active_cell = 0;
  _evalCellAction->setEnabled(false);
}


void
Notebook::onCellModifiedStateChanged(bool state) {
  emit modifiedStateChanged();
}


void
Notebook::onSave() {
  if (! hasFileName()) {
    onSaveAs(); return;
  }

  QFile file(fileName());
  if(! file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::critical(0, tr("Can not save file"), tr("Can not save as %1.").arg(fileName()));
    return;
  }

  // Serialize
  for (QList<Cell *>::iterator cell=_cells.begin(); cell!=_cells.end(); cell++) {
    file.write((*cell)->codeDocument()->toPlainText().toAscii());
    if ((*cell) != _cells.back()) {
      file.write("\n# -*- snip -*-\n");
    }
    (*cell)->setModified(false);
  }
  file.close();
}


void
Notebook::onSaveAs() {
  QString file_name = QFileDialog::getSaveFileName(0, tr("Save as..."), "", "Python Code Files (*.py)");
  if (0 == file_name.size()) { return; }
  setFileName(file_name);
  onSave();
}


void
Notebook::onUndo() {
  if (0 == _active_cell) { return; }
  _active_cell->codeDocument()->undo();
}

void
Notebook::onRedo() {
  if (0 == _active_cell) { return; }
  _active_cell->codeDocument()->redo();
}

void
Notebook::onNewCell()
{
  // If no cell is selected -> append cell
  if (0 == _active_cell) {
    Cell *cell = new Cell(this);
    QObject::connect(cell, SIGNAL(cellActivated(Cell*)), this, SLOT(onCellActivated(Cell*)));
    QObject::connect(cell, SIGNAL(cellDeactivated(Cell*)), this, SLOT(onCellDeactivated(Cell*)));
    QObject::connect(cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));
    _cells.append(cell);
    emit cellAdded(_cells.size()-1, cell);
    return;
  }

  // Otherwise insert cell after active cell
  Cell *cell = new Cell(this);
  QObject::connect(cell, SIGNAL(cellActivated(Cell*)), this, SLOT(onCellActivated(Cell*)));
  QObject::connect(cell, SIGNAL(cellDeactivated(Cell*)), this, SLOT(onCellDeactivated(Cell*)));
  QObject::connect(cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));
  int idx =_cells.indexOf(_active_cell)+1;
  _cells.insert(idx, cell);
  emit cellAdded(idx, cell);
}

void
Notebook::onDeleteCell()
{
  // If there is no active cell
  if (0 == _active_cell) { return; }

  // If the active cell is evaluated or event queued to be evaluated:
  if ( (Cell::QUEUED == _active_cell->evaluationState()) ||
       (Cell::EVALUATING == _active_cell->evaluationState()) ) {
    return;
  }

  Cell *cell = _active_cell;
  QObject::disconnect(cell, 0, this, 0);

  int idx = _cells.indexOf(cell);
  emit cellRemoved(idx);
  QApplication::instance()->processEvents();

  _cells.removeAt(idx);
  cell->deleteLater();
  _active_cell = 0;
}

void
Notebook::onSplitCell() {
  if (0 == _active_cell) { return; }
  int idx = _cells.indexOf(_active_cell);
  int split_at = _active_cell->splitPosition();

  QString text = _active_cell->codeDocument()->toPlainText();
  _active_cell->codeDocument()->setPlainText(text.left(split_at));

  Cell *new_cell = new Cell(this);
  new_cell->codeDocument()->setPlainText(text.mid(split_at));
  QObject::connect(new_cell, SIGNAL(cellActivated(Cell*)), this, SLOT(onCellActivated(Cell*)));
  QObject::connect(new_cell, SIGNAL(cellDeactivated(Cell*)), this, SLOT(onCellDeactivated(Cell*)));
  QObject::connect(new_cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));
  _cells.insert(idx+1, new_cell);
  emit cellAdded(idx+1, new_cell);
}


void
Notebook::onJoinCell() {
  if (0 == _active_cell) { return; }
  int idx = _cells.indexOf(_active_cell);
  if (idx == _cells.size()-1) { return; }
  Cell *next_cell = _cells.at(idx+1);

  // If the next cell is evaluated or event queued to be evaluated:
  if ( (Cell::QUEUED == next_cell->evaluationState()) ||
       (Cell::EVALUATING == next_cell->evaluationState()) ) {
    return;
  }

  QString text = _active_cell->codeDocument()->toPlainText();
  text.append("\n"); text.append(next_cell->codeDocument()->toPlainText());
  _active_cell->codeDocument()->setPlainText(text);

  QObject::disconnect(next_cell, 0, this, 0);
  emit cellRemoved(idx+1);
  QApplication::instance()->processEvents();

  _cells.removeAt(idx+1);
  next_cell->deleteLater();
}


void
Notebook::onEvalCell() {
  if (0 == _active_cell) { return; }
  // Clear result cell:
  _active_cell->resultDocument()->clear();
  PythonEngine::get()->queueCell(_active_cell);
}

void
Notebook::onEvalAllCells() {
  for(QList<Cell *>::iterator cell=_cells.begin(); cell!=_cells.end(); cell++) {
    (*cell)->resultDocument()->clear();
    PythonEngine::get()->queueCell(*cell);
  }
}
