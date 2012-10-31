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

#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include "cell.hh"
//#include "preamblecell.hh"

#include <QScrollArea>
#include <QList>
#include <QLayout>
#include <QAction>

#include <Python.h>


/** A notebook is a list of cells representing a code file. */
class Notebook : public QObject
{
    Q_OBJECT

public:
  /** Constructs a new, empty notebook. */
  explicit Notebook(QObject *parent = 0);

  /** Constructs a notebook from the given file. */
  explicit Notebook(const QString &path, QObject *parent = 0);

  /** Returns the context of the Notebook */
  PythonContext *context();

  bool hasFileName() const;
  void setFileName(const QString &filename);
  const QString &fileName() const;

  size_t numCells() const;
  Cell *cell(size_t i);

  inline QAction *saveNotebookAction() { return _saveAction; }
  inline QAction *saveNotebookAsAction() { return _saveAsAction; }
  inline QAction *printNotebookAction() { return _printAction; }

  inline QAction *undoAction() { return _undoAction; }
  inline QAction *redoAction() { return _redoAction; }

  inline QAction *newCellAction() { return _newCellAction; }
  inline QAction *deleteCellAction() { return _deleteCellAction; }
  inline QAction *splitCellAction() { return _splitCellAction; }
  inline QAction *joinCellsAction() { return _joinCellsAction; }
  inline QAction *evalCellAction() { return _evalCellAction; }
  inline QAction *evalAllCellsAction() { return _evalAllCellsAction; }

  bool isModified() const;

signals:
  void cellAdded(int index, Cell *cell);
  void cellRemoved(int index);
  void modifiedStateChanged();

private slots:
  void onCellActivated(Cell *cell);
  void onCellDeactivated(Cell *cell);
  void onCellModifiedStateChanged(bool state);

  void onUndo();
  void onRedo();

  void onSave();
  void onSaveAs();

  void onNewCell();
  void onDeleteCell();
  void onSplitCell();
  void onJoinCell();
  void onEvalCell();
  void onEvalAllCells();

private:
  /** Holds all cells of the notebook. */
  QList<Cell *> _cells;
  /** The current active cell (the cell with KB focus). */
  Cell *_active_cell;

  /** Holds the filepath if one is assigned to the notebook. */
  QString _filepath;
  /** The python context (namespaces and scopes) of the notebook. */
  PythonContext *_python_context;
  QAction *_saveAction;
  QAction *_saveAsAction;
  QAction *_printAction;
  QAction *_undoAction;
  QAction *_redoAction;
  QAction *_newCellAction;
  QAction *_deleteCellAction;
  QAction *_splitCellAction;
  QAction *_joinCellsAction;
  QAction *_evalCellAction;
  QAction *_evalAllCellsAction;

private:
  void _createActions();
};

#endif // NOTEBOOK_H
