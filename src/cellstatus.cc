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

#include "cellstatus.hh"

CellStatus::CellStatus(QWidget *parent) :
    QWidget(parent)
{
    //
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    // set minimum width to 5px:
    this->setMinimumWidth(5);
    this->resize(5,5);

    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::lightGray);
    this->setPalette(pal);
    this->setAutoFillBackground(true);
}


void
CellStatus::setStatusSuccess()
{
    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::green);
    this->setPalette(pal);
}


void
CellStatus::setStatusRunning()
{
    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::yellow);
    this->setPalette(pal);
}


void
CellStatus::setStatusError()
{
    // Set background color.
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::red);
    this->setPalette(pal);
}

