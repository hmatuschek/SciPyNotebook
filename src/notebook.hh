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

  bool isModified() const;

signals:
  void cellAdded(int index, Cell *cell);
  void cellRemoved(int index);
  void modifiedStateChanged();

public slots:
  void save();
  void saveAs();
  void newCell(int index);
  void deleteCell(int index);
  void splitCell(int index, int off);
  void joinCell(int index);
  void evalCell(int index);
  void evalAllCells();

private slots:
  void onCellModifiedStateChanged(bool state);

private:
  /** Holds all cells of the notebook. */
  QList<Cell *> _cells;
  /** Holds the filepath if one is assigned to the notebook. */
  QString _filepath;
  /** The python context (namespaces and scopes) of the notebook. */
  PythonContext *_python_context;
};

#endif // NOTEBOOK_H
