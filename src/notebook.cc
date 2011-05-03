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
#include "preferences.hh"

#include <QApplication>
#include <QFile>


Notebook::Notebook(QWidget *parent) :
    QFrame(parent)
{
    // initialize python context of notebook:
    this->_python_context = new PythonContext();

    // initialize notebook layout and connect signals
    this->initNotebookLayout();

    // Create an empty cell
    Cell *new_cell = new Cell(this);

    // set preamble code from preferences
    new_cell->setCode(Preferences::get()->preamble());

    // Connect to signals
    QObject::connect(new_cell, SIGNAL(makeVisible(QPoint)),
                     this, SLOT(makeCellVisible(QPoint)));
    QObject::connect(new_cell, SIGNAL(cellChanged()),
                     this, SLOT(cellModified()));

    // Append cell to list:
    this->_cell_layout->addWidget(new_cell);
    this->_cells.append(new_cell);

    // Set focus to new cell
    new_cell->setFocus();
}


Notebook::Notebook(const QString &filename, QWidget *parent) :
    QFrame(parent)
{
  // initialize python context of notebook:
  this->_python_context = new PythonContext();
  this->_python_context->setFileName(filename);

  // initialize notebook layout and connect signals
  this->initNotebookLayout();

  // Open file...
  this->_filename = filename;
  QFile file(this->_filename);
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  // Check if file is open for reading:
  if(! file.isOpen())
  {
      qCritical("Oops, file (%s) not open...", filename.toStdString().c_str());
      exit(-1);
  }

  // Read file content line-by-line
  QRegExp cellSepExpr("^#\\s+-\\*-\\s+snip\\s+-\\*-\\s*$");
  QList<QByteArray> cells;
  cells.append(QByteArray());

  while(! file.atEnd())
  {
    // Read line:
    QByteArray line = file.readLine();

    // check if line is cell separator
    if (cellSepExpr.indexIn(line) != -1)
    {
      // add an new cell to list
      cells.append(QByteArray());
    }
    else
    {
      // append line to current cell
      cells.back().append(line);
    }
  }

  // Create cells from code:
  foreach (QByteArray code, cells)
  {
    Cell *cell = new Cell(this);
    this->_cells.append(cell);
    this->_cell_layout->addWidget(cell);

    cell->setCode(code.trimmed());

    QObject::connect(cell, SIGNAL(makeVisible(QPoint)),
                     this, SLOT(makeCellVisible(QPoint)));    
    QObject::connect(cell, SIGNAL(cellChanged()),
                     this, SLOT(cellModified()));
  }
}


void
Notebook::initNotebookLayout()
{
  this->_cell_layout = new QVBoxLayout();
  this->_cell_layout->setSpacing(0);
  this->_cell_layout->setContentsMargins(0,0,0,0);
  this->setLayout(this->_cell_layout);
  this->_cell_layout->setSizeConstraint(QLayout::SetMinimumSize);
  this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
}


void
Notebook::onNewCell()
{
  int index = -1;

  // Get the current cell and its index (if known)
  if (0 != QApplication::focusWidget())
  {
    QWidget *w = (QWidget *)(QApplication::focusWidget()->parent());
    index = this->layout()->indexOf(w);

    // If focus widget is not in notebook layout
    if(0 <= index)
      index++;
  }

  //Append a new cell
  Cell *new_cell = new Cell(this);
  new_cell->setFocus();
  QObject::connect(new_cell, SIGNAL(makeVisible(QPoint)),
                   this, SLOT(makeCellVisible(QPoint)));
  QObject::connect(new_cell, SIGNAL(cellChanged()),
                   this, SLOT(cellModified()));

  // Insert cell
  if (0 > index)
  {
    this->_cell_layout->addWidget(new_cell);
    this->_cells.push_back(new_cell);
  }
  else
  {
    this->_cell_layout->insertWidget(index, new_cell);
    this->_cells.insert(index, new_cell);
  }
}


bool
Notebook::hasFileName()
{
  return 0 != this->_filename.size();
}


const QString &
Notebook::fileName()
{
  return this->_filename;
}


void
Notebook::setFileName(const QString &filename)
{
  this->_filename = filename;
  this->_python_context->setFileName(filename);
}


PythonContext *
Notebook::pythonContext()
{
  return this->_python_context;
}


bool
Notebook::isModified()
{
  return this->_is_modified;
}


void
Notebook::evalCellSlot()
{
  // Get selected cell:
  Cell *cell = (Cell *)(QApplication::focusWidget()->parent());

  // Check if w is a Cell object and if it is associated with this notebook.
  if(-1 == this->layout()->indexOf(cell))
    return;

  cell->evaluate(this->_python_context);
}


void
Notebook::evalAllCellsSlot()
{
  // Evaluate all cells in order
  foreach(Cell *cell, this->_cells)
  {
    // Evaluate cell but stop iteration if evaluation fails
    if (! cell->evaluate(this->_python_context))
    {
      return;
    }
  }
}


void
Notebook::save()
{
  // Do nothing if no filename is set...
  if (!this->hasFileName())
  {
    return;
  }

  // open file;
  QFile file(this->_filename);

  /// \todo Show message if file can not be saved.
  file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

  size_t index = 1;
  foreach(Cell *cell, this->_cells)
  {
    // Write code into file;
    cell->serializeCode(file);

    // Write cell separator if not last cell
    if (index != this->_cells.size())
    {
      /// \todo Make lineendings more platform specific.

      file.write("\n");
      file.write("# -*- snip -*-");
      file.write("\n");
    }

    index++;
  }

  // emit the "isSaved" signal
  this->_is_modified = false;
  emit this->saved();
}


void
Notebook::undoSlot()
{
  // Get the current cell and its index (if known)
  QWidget *w = (QWidget *)(QApplication::focusWidget()->parent());
  int index = this->layout()->indexOf(w);

  if(0 > index)
      return;

  // forward to current cell
  this->_cells[index]->undoSlot();
}


void
Notebook::redoSlot()
{
  // Get the current cell and its index (if known)
  QWidget *w = (QWidget *)(QApplication::focusWidget()->parent());
  int index = this->layout()->indexOf(w);

  if(0 > index)
      return;

  // forward to current cell
  this->_cells[index]->redoSlot();
}


void
Notebook::splitCellSlot()
{
  // Get the current cell and its index (if known)
  QWidget *w = (QWidget *)(QApplication::focusWidget()->parent());
  int index = this->layout()->indexOf(w);

  // If no cell has focus, done.
  if(0 > index)
      return;

  // Cast to cell widget
  Cell *cell = (Cell *)w;

  // Get cursor position and split text
  int pos = cell->codecell->textCursor().position();
  QString text = cell->codecell->document()->toPlainText();
  QString text1 = text.left(pos);
  QString text2 = text.right(text.length()-pos);

  // Reset text of "old cell"
  cell->setCode(text1);

  // Create new cell and add its text:
  cell = new Cell(this);
  QObject::connect(cell, SIGNAL(makeVisible(QPoint)),
                   this, SLOT(makeCellVisible(QPoint)));
  QObject::connect(cell, SIGNAL(cellChanged()),
                   this, SLOT(cellModified()));

  // Add "new" cell to layout and list of cells:
  this->_cell_layout->insertWidget(index+1, cell);
  this->_cells.insert(index+1, cell);

  // Set code of second (new) cell
  cell->setCode(text2);

  // signal modification
  this->cellModified();
}


void
Notebook::joinCellsSlot()
{
    // Get the current cell and its index (if known)
    QWidget *w = (QWidget *)(QApplication::focusWidget()->parent());
    int index = this->layout()->indexOf(w);

    // If no cell has focus or the last cell has the focus:
    if(0 > index || index == this->_cells.size()-1)
        return;

    // Get cell at index:
    Cell *cell1 = this->_cells[index];
    Cell *cell2 = this->_cells[index+1];

    // Get and join code:
    /// \todo Make line break platform depended.
    QString code = cell1->codecell->document()->toPlainText()
                   + "\n"
                   + cell2->codecell->document()->toPlainText();
    cell1->setCode(code);

    // remove widget from layout and list:
    this->layout()->removeWidget(cell2);
    this->_cells.removeAt(index+1);

    // Disconnect from all signals of the cell
    QObject::disconnect(cell2, 0, this, 0);

    // free it
    delete cell2;

    // signal modification
    this->cellModified();
}


void
Notebook::delCellSlot()
{
    // Get the current cell and its index (if known)
    QWidget *w = (QWidget *)(QApplication::focusWidget()->parent());
    int index = this->layout()->indexOf(w);

    // If no cell has focus:
    if(0 > index)
        return;

    // Get cell at index:
    Cell *cell = this->_cells[index];

    // Remove cell from list and layout:
    this->layout()->removeWidget(cell);
    this->_cells.removeAt(index);

    // Disconnect from all signals of the cell
    QObject::disconnect(cell, 0, this, 0);

    // free cell
    delete cell;

    // signal modification
    this->cellModified();
}


void
Notebook::makeCellVisible(QPoint coord)
{
  emit this->makeVisible(coord);
}


void
Notebook::cellModified()
{
  if (! this->_is_modified)
    emit this->modified();
}

