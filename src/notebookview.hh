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
  explicit NotebookView(QWidget *parent = 0);
  void setNotebook(Notebook *notebook);
  Notebook *notebook();

public slots:
  void undo();
  void redo();
  void copy();
  void cut();
  void paste();

  void newCell();
  void deleteCell();
  void splitCell();
  void joinCell();
  void evalCell();

private slots:
  void onCellAdded(int index, Cell *cell);
  void onCellRemoved(int index);
  void onCellActivated(CellView *cell);
  void onCellDeactivated(CellView *cell);

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
