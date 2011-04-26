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

#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QVBoxLayout>

#include "codecell.hh"
#include "resultcell.hh"
#include "cellstatus.hh"



class Cell : public QFrame
{
    Q_OBJECT

  friend class Notebook;


protected:
    QBoxLayout *cellbox;
    CodeCell *codecell;
    ResultCell *resultcell;
    CellStatus *cell_status;


public:
    explicit Cell(QWidget *parent=0);

    void evaluate();
    virtual void setFocus();

    void serializeCode(QIODevice &device);
    void setCode(const QString &code);


public slots:
    void undoSlot();
    void redoSlot();
};

#endif // CELL_H
