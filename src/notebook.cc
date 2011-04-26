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

#include <QApplication>
#include <QFile>

#include <iostream>


Notebook::Notebook(QWidget *parent) :
    QFrame(parent)
{
    // initialize notebook layout and connect signals
    this->initNotebookLayout();

    // Create an empty cell
    Cell *new_cell = new Cell(this);
    //new_cell->setFocus();
    this->_cell_layout->addWidget(new_cell);

    // Append cell to list:
    this->_cells.append(new_cell);
}


Notebook::Notebook(const QString &filename, QWidget *parent) :
    QFrame(parent)
{
  // initialize notebook layout and connect signals
  this->initNotebookLayout();

  // Open file...
  this->_filename = filename;
  QFile file(this->_filename);
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  // Check if file is open for reading:
  if(! file.isOpen())
  {
    std::cerr << "Oops, file (" << filename.toStdString() << ")not open..." << std::endl;
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
      std::cerr << "Add new cell..." << std::endl;
      // add an new cell to list
      cells.append(QByteArray());
    }
    else
    {
      std::cerr << "Add line to current cell..." << std::endl;
      // append line to current cell
      cells.back().append(line);
    }
  }

  // Create cells from code:
  foreach (QByteArray code, cells)
  {
    std::cerr << "Create cell..." << std::endl;
    Cell *cell = new Cell();
    this->_cells.append(cell);
    this->_cell_layout->addWidget(cell);
    cell->setCode(code);
  }

  std::cerr << "Done..." << std::endl;
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
Notebook::onEvalCell()
{
    //Eval cell
    Cell *cell = (Cell *)(QApplication::focusWidget()->parent());

    // Check if w is a Cell object and if it is associated with this notebook.
    if(-1 == this->layout()->indexOf(cell))
        return;

    cell->evaluate();
}


void
Notebook::onNewCell()
{
    // Get the current cell and its index (if known)
    QWidget *w = (QWidget *)(QApplication::focusWidget()->parent());
    int index = this->layout()->indexOf(w);
    if(0 <= index)
        index++;

    //Append a new cell
    Cell *new_cell = new Cell();
    new_cell->setFocus();

    // Insert cell
    this->_cell_layout->insertWidget(index, new_cell);
    if (0 > index)
        this->_cells.push_back(new_cell);
    else
        this->_cells.insert(index, new_cell);
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
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    /// \todo Serialize preamble!

    size_t index = 1;
    foreach(Cell *cell, this->_cells)
    {
        // Write code into file;
        cell->serializeCode(file);

        if (index != this->_cells.size())
        {
            /// \todo Make lineendings more platform specific.
            file.write("\n");
            file.write("# -*- snip -*-");
            file.write("\n");
        }

        index++;
    }
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
  cell = new Cell();

  // Add "new" cell to layout and list of cells:
  this->_cell_layout->insertWidget(index+1, cell);
  this->_cells.insert(index, cell);

  cell->setCode(text2);
}
