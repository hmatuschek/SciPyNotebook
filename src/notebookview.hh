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

#include <QScrollArea>
#include <QList>
#include <QLayout>

#include <Python.h>


/**
 * The notebook view class.
 */
class NotebookView : public QFrame
{
  Q_OBJECT

protected:
  /** Holds the notebook instance being displayed. */
  Notebook *_notebook;
  /** The BoxLayout to layout the cells. */
  QBoxLayout *_cell_layout;

public:
  explicit NotebookView(QWidget *parent = 0);

  void setNotebook(Notebook *notebook);
  Notebook *notebook();

private slots:
  void onCellAdded(int index, Cell *cell);
  void onCellRemoved(int index);
};

#endif // NOTEBOOKVIEW_H
