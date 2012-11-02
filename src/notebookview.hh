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

#ifndef NOTEBOOKVIEW_H
#define NOTEBOOKVIEW_H

#include "notebook.hh"
#include "cellview.hh"

#include <QScrollArea>
#include <QList>
#include <QLayout>

/**
 * The notebook view class.
 */
class NotebookView : public QFrame
{
  Q_OBJECT

public:
  /** Constructs an empty notebook view. Use @c setNotebook to set the notebook to display. */
  explicit NotebookView(QWidget *parent = 0);
  /** (Re-) Sets the notebook being displayed. */
  void setNotebook(Notebook *notebook);
  /** Retruns a weak reference to the displayed notebook. */
  Notebook *notebook();

public slots:
  /** Undoes the last action in the current active cell. */
  void undo();
  /** Redoes the last action in the current active cell. */
  void redo();
  /** Copies the selected text in the current active cell. */
  void copy();
  /** Cuts the selected text from the current active cell. */
  void cut();
  /** Pastes some text from the clipboard into the active cell. */
  void paste();

  /** Creates a new cell below the active cell or at the end of the notebook if no cell is
   * currently active. */
  void newCell();
  /** Deletes the currently active cell if the cell is not being evaluated or is scheduled for
   * execution. */
  void deleteCell();
  /** Splits the current cell at the current line of the cursor. */
  void splitCell();
  /** Joins the current and the next cell if the next cell is not being evaluated or is scheduled
   * for execution. */
  void joinCell();
  /** Evaluates the currently active cell. */
  void evalCell();

private slots:
  /** Updates the view if a new cell is added to the notebook. */
  void onCellAdded(int index, Cell *cell);
  /** Updates the view if a cell is removed from the notebook. */
  void onCellRemoved(int index);
  /** Sets this cell to be the currently active cell. */
  void onCellActivated(CellView *cell);
  /** Decativates the currently active cell. */
  void onCellDeactivated(CellView *cell);
  /** Activates the cell right after the currently active cell. */
  void onActivateNextCell(CellView *cell);
  /** Activates the cell right before the currently active cell. */
  void onActivatePrevCell(CellView *cell);

protected:
  /** Holds the notebook instance being displayed. */
  Notebook *_notebook;
  /** Holds a list of weak references to the @c CellView instances of the notebook view. */
  QList<CellView *> _cells;
  /** The BoxLayout to layout the cells. */
  QBoxLayout *_cell_layout;
  /** Holds the currently active cell. */
  CellView *_active_cell;
};

#endif // NOTEBOOKVIEW_H
