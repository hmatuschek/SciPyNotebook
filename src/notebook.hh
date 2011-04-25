/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QScrollArea>
#include <QList>
#include <QLayout>

#include "cell.hh"


class Notebook : public QFrame
{
    Q_OBJECT

protected:
    QList<Cell *> _cells;
    QShortcut *_eval_shortcut;
    QShortcut *_new_cell_shortcut;
    QBoxLayout *cell_layout;
    QString _filename;


public:
    explicit Notebook(QWidget *parent = 0);

    bool hasFileName();
    const QString &fileName();
    void setFileName(const QString &filename);

    void save();


public slots:
    void onEvalCell();
    void onNewCell();
};

#endif // NOTEBOOK_H
