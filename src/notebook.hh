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

#include <Python.h>


/**
 * The notebook class.
 */
class Notebook : public QFrame
{
    Q_OBJECT

protected:
  /**
   * Holds all cells of the notebook.
   */
  QList<Cell *> _cells;

  /**
   * The BoxLayout to layout the cells.
   */
  QBoxLayout *_cell_layout;

  /**
   * Holds the filename of the notebook.
   *
   * Can be set/get via filename(), setFilename();
   */
  QString _filename;

  /**
   * Is true if the notebook is not saved, that means if it was modified since the last
   * save.
   */
  bool _is_modified;

  /**
   * The python context (namespaces and scopes) of the notebook.
   */
  PythonContext *_python_context;


public:
    /**
     * Constructs a new, empty notebook.
     */
    explicit Notebook(QWidget *parent = 0);

    /**
     * Constructs a new empty notebook from the given file.
     */
    Notebook(const QString &filename, QWidget *parent);

    /**
     * Returns true, if a filename is associated with the notebook.
     */
    bool hasFileName();

    /**
     * Returns the filename associated with the notebook.
     */
    const QString &fileName();

    /**
     * (Re-) Sets the filename associated with the note book.
     *
     * You need to call save() to save the notebook content into the file.
     */
    void setFileName(const QString &filename);

    /**
     * Returns true if the notebook was modified since the last save.
     */
    bool isModified();


protected:
    /**
     * Internal helper function to setup the layout of the notebook.
     */
    void initNotebookLayout();


signals:
    /**
     * This signal is emitted if the given part of the notebook needs to be visible.
     */
    void makeVisible(QPoint coord);

    /**
     * This signal is emitted, if the one of the cells is modified since the last save.
     */
    void modified();

    /**
     * This signal is emitted, if the notebook was saved.
     */
    void saved();


public slots:
    /**
     * Saves the notebook to the given file.
     *
     * If no filename is associated with the notebook, nothing will be done.
     */
    void save();

    void evalCellSlot();
    void evalAllCellsSlot();

    void onNewCell();

    void undoSlot();
    void redoSlot();

    void splitCellSlot();
    void joinCellsSlot();
    void delCellSlot();


  protected slots:
    /**
     * Is connected to all cells to emit a makeVisible signal.
     */
    void makeCellVisible(QPoint coord);

    /**
     * Is connected to all cells to emit a isModified() signal.
     */
    void cellModified();
};

#endif // NOTEBOOK_H
