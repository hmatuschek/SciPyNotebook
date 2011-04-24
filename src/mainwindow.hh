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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>

#include "notebook.hh"


class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    /**
     * Holds the reference to the console widget.
     */
    Notebook *notebook;

    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *printAct;
    QAction *quitAct;

    QMenu *editMenu;
    QAction *undoAct;
    QAction *redoAct;
    QAction *copyAct;
    QAction *cutAct;
    QAction *pasteAct;
    QAction *prefAct;


public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
};

#endif // MAINWINDOW_H
