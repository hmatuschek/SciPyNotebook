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

#ifndef NOTEBOOKWINDOW_HH
#define NOTEBOOKWINDOW_HH

#include "notebook.hh"

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "notebookview.hh"


/** Holds the notebook view and a menu bar. */
class NotebookWindow : public QMainWindow
{
  Q_OBJECT

protected:
  NotebookView *_notebook_view;
  QScrollArea *_scrolledWindow;

  QMenu *_fileMenu;
  QMenu *_editMenu;
  QMenu *_cellMenu;
  QMenu *_helpMenu;


public:
  /** Constructs a new notebook window with an empty notebook. */
  explicit NotebookWindow(QWidget *parent = 0);
  NotebookWindow(const QString &filename, QWidget *parent=0);
  virtual ~NotebookWindow();

  Notebook *notebook();

protected:
  /** Assembles the notebook window. */
  void _initNotebookWindow();

private slots:
  void updateWindowTitle();
  void onCloseNotebook();

};

#endif // NOTEBOOKWINDOW_HH
