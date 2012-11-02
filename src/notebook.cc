/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011, 2012 Hannes Matuschek <hmatuschek AT gmail DOT com>
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
  QObject::connect(cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));
}


Notebook::Notebook(const QString &path, QObject *parent)
  : QObject(parent), _filepath(path), _python_context(0)
{
  // Create new empty context
  _python_context = new PythonContext(this);
  _python_context->setFileName(path);

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
    QObject::connect(cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));
  }
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
  _python_context->setFileName(_filepath);
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
Notebook::onCellModifiedStateChanged(bool state) {
  emit modifiedStateChanged();
}


void
Notebook::save() {
  if (! hasFileName()) {
    saveAs(); return;
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
Notebook::saveAs() {
  QString file_name = QFileDialog::getSaveFileName(0, tr("Save as..."), "", "Python Code Files (*.py)");
  if (0 == file_name.size()) { return; }
  setFileName(file_name);
  save();
}


void
Notebook::newCell(int index)
{
  if (0 > index) { return; }
  Cell *cell = new Cell(this);
  QObject::connect(cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));
  _cells.insert(index, cell);
  emit cellAdded(index, cell);
}

void
Notebook::deleteCell(int index)
{
  if (0 > index) { return; }
  Cell *active_cell = _cells.at(index);

  // If the active cell is evaluated or event queued to be evaluated:
  if ( (Cell::QUEUED == active_cell->evaluationState()) ||
       (Cell::EVALUATING == active_cell->evaluationState()) ) {
    return;
  }

  QObject::disconnect(active_cell, 0, this, 0);
  emit cellRemoved(index);
  QApplication::instance()->processEvents();

  _cells.removeAt(index);
  active_cell->deleteLater();
}

void
Notebook::splitCell(int index, int split_at) {
  if (0 > index) { return; }

  Cell *active_cell = _cells.at(index);
  QString text = active_cell->codeDocument()->toPlainText();
  active_cell->codeDocument()->setPlainText(text.left(split_at));

  Cell *new_cell = new Cell(this);
  new_cell->codeDocument()->setPlainText(text.mid(split_at));
  QObject::connect(new_cell, SIGNAL(modifiedStateChanged(bool)), this, SLOT(onCellModifiedStateChanged(bool)));
  _cells.insert(index+1, new_cell);
  emit cellAdded(index+1, new_cell);
}


void
Notebook::joinCell(int index)
{
  if (0 > index) { return; }
  if (index == (_cells.size()-1)) { return; }
  Cell *active_cell = _cells.at(index);
  Cell *next_cell = _cells.at(index+1);

  // If the next cell is evaluated or event queued to be evaluated:
  if ( (Cell::QUEUED == next_cell->evaluationState()) ||
       (Cell::EVALUATING == next_cell->evaluationState()) ) {
    return;
  }

  QString text = active_cell->codeDocument()->toPlainText();
  text.append("\n"); text.append(next_cell->codeDocument()->toPlainText());
  active_cell->codeDocument()->setPlainText(text);

  QObject::disconnect(next_cell, 0, this, 0);
  emit cellRemoved(index+1);
  QApplication::instance()->processEvents();

  _cells.removeAt(index+1);
  next_cell->deleteLater();
}


void
Notebook::evalCell(int index) {
  if (0 > index) { return; }
  Cell *active_cell = _cells.at(index);
  // Clear result cell:
  active_cell->resultDocument()->clear();
  PythonEngine::get()->queueCell(active_cell);
}

void
Notebook::evalAllCells() {
  for(QList<Cell *>::iterator cell=_cells.begin(); cell!=_cells.end(); cell++) {
    (*cell)->resultDocument()->clear();
    PythonEngine::get()->queueCell(*cell);
  }
}
