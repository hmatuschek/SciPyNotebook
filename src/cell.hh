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

#ifndef __SCIPY_NOTEBOOK_CELL_H__
#define __SCIPY_NOTEBOOK_CELL_H__

#include <QObject>
#include <QVBoxLayout>

#include "codecell.hh"
#include "resultcell.hh"
#include "cellstatus.hh"
#include "pythoncontext.hh"


/**
 * The cell class combines two cells, one for the input of code and another for the
 * output, this code generates or error messages.
 */
class Cell : public QFrame
{
  Q_OBJECT

  /**
   * \todo May not be needed in future.
   */
  friend class Notebook;


protected:
  /**
   * The layout for the code and result cells.
   */
  QBoxLayout *cellbox;

  /**
   * Holds the instance of the code cell.
   */
  CodeCell *codecell;

  /**
   * Holds the instance of the result cell.
   */
  ResultCell *resultcell;

  /**
   * A simple widget to show the status of the cell.
   */
  CellStatus *cell_status;


public:
  /**
   * Construct a new (empty) cell.
   */
  explicit Cell(QWidget *parent=0);

  /**
   * Evaluates the cell in the given PythonContext.
   */
  void evaluate(PythonContext *ctx);

  /**
   * Serializes the code of the codecell into the given device.
   */
  void serializeCode(QIODevice &device);

  /**
   * Replaces the code in the codecell with the given text.
   */
  void setCode(const QString &code);


signals:
  void statusClicked();


public slots:
  /**
   * Forward to codecell.undo().
   */
  void undoSlot();

  /**
   * Forward to codecell.redo().
   */
  void redoSlot();


protected slots:
  void onStatusClicked();
};

#endif // __SCIPY_NOTEBOOK_CELL_H__
