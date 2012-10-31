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

#ifndef CELLSTATUS_HH
#define CELLSTATUS_HH

#include <QWidget>

class CellStatus : public QWidget
{
    Q_OBJECT

public:
    explicit CellStatus(QWidget *parent = 0);

    void setStatusSuccess();
    void setStatusError();
    void setStatusQueued();
    void setStatusRunning();
    void setStatusModified();

    virtual void mouseReleaseEvent(QMouseEvent *event);


signals:
    void clicked();
};

#endif // CELLSTATUS_HH
