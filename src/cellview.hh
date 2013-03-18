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

#ifndef __SCIPY_NOTEBOOK_CELLVIEW_H__
#define __SCIPY_NOTEBOOK_CELLVIEW_H__

#include <QObject>
#include <QVBoxLayout>

#include "cell.hh"
#include "codecell.hh"
#include "resultcell.hh"
#include "cellstatus.hh"
#include "pythoncontext.hh"
#include "pythonhighlighter.hh"

/* forward declarations. */
class Notebook;
class Cell;


/**
 * The cell class combines two cells, one for the input of code and another for the
 * output the code generates or error messages.
 */
class CellView : public QFrame
{
  Q_OBJECT

public:
  /** Creates a new view for the given cell. */
  explicit CellView(Cell *cell, QWidget *parent=0);
  /** Destructor. */
  virtual ~CellView();

  /** Returns the cell of the view. */
  Cell *cell();

  /** Returns the offset in chars of the beginning of the current line.*/
  int currentLineOffset();

public slots:
  void undo();
  void redo();
  void copy();
  void cut();
  void paste();

signals:
  void cellActivated(CellView *cell);
  void cellDeactivated(CellView *cell);
  void activateNextCell(CellView *cell);
  void activatePrevCell(CellView *cell);

private slots:
  void onCellEvaluationStateChanged(unsigned int last_state, unsigned int new_state);
  void onCellDeleted();
  void onHighlightLine(int line);
  void onCellActivated(CodeCell *cell);
  void onCellDeactivated(CodeCell *cell);
  void onActivateNextCell();
  void onActivatePrevCell();

private:
  /** Holds a weak reference to the cell object. */
  Cell *_cell;
  /** Holds the code cell view. */
  CodeCell *_codecell;
  /** Holds the result cell view. */
  ResultCell *_resultcell;
  /** The layout for the code and result cells. */
  QBoxLayout *_cellbox;
  /** A simple widget to show the status of the cell. */
  CellStatus *_cell_status;
};

#endif // __SCIPY_NOTEBOOK_CELLVIEW_H__
