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

#include <QScrollArea>
#include <QList>
#include <QLayout>

#include "cell.hh"


/**
 * The notebook class.
 */
class Notebook : public QFrame
{
    Q_OBJECT

protected:
    QList<Cell *> _cells;
    QShortcut *_eval_shortcut;
    QShortcut *_new_cell_shortcut;
    QBoxLayout *_cell_layout;
    QString _filename;


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


protected:
    void initNotebookLayout();


public slots:
    /**
     * Saves the notebook to the given file.
     *
     * If no filename is associated with the notebook, nothing will be done.
     */
    void save();

    void onEvalCell();
    void onNewCell();

    void undoSlot();
    void redoSlot();

    void splitCellSlot();
    void joinCellsSlot();
};

#endif // NOTEBOOK_H
