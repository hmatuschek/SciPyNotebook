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

#include "mainwindow.hh"

#include <QScrollArea>
#include <QMenuBar>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Set window title
    this->setWindowTitle("SciPy Notebook");
    this->resize(533, 640);

    // Create new notebook widget...
    this->notebook = new Notebook();
    QScrollArea *swin = new QScrollArea();
    swin->setWidget(this->notebook);
    swin->setWidgetResizable(true);

    // layout the stuff...
    this->setCentralWidget(swin);

    // Assemble actions:
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("A new, empty notebook."));

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a notebook or python script."));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the notebook."));

    saveAsAct = new QAction(tr("Save &As"), this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the notbook under a different name."));

    printAct = new QAction(tr("&Print"), this);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the notebook."));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcut(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit SciPy Notebook."));

    undoAct = new QAction(tr("Undo"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo last changes."));

    redoAct = new QAction(tr("Redo"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo last changes."));

    copyAct = new QAction(tr("Copy"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy selected text."));

    cutAct  = new QAction(tr("Cut"), this);
    cutAct->setShortcut(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut selected text."));

    pasteAct = new QAction(tr("Paste"), this);
    pasteAct->setShortcut(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste selected text."));

    prefAct = new QAction(tr("Preferences"), this);
    prefAct->setShortcut(QKeySequence::Preferences);
    prefAct->setStatusTip(tr("Edit the preferences."));

    // Assemble menu:
    fileMenu = this->menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(this->newAct);
    fileMenu->addAction(this->openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(this->saveAct);
    fileMenu->addAction(this->saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(this->printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(this->quitAct);

    editMenu = this->menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(this->undoAct);
    editMenu->addAction(this->redoAct);
    editMenu->addSeparator();
    editMenu->addAction(this->copyAct);
    editMenu->addAction(this->cutAct);
    editMenu->addAction(this->pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(this->prefAct);
}



MainWindow::~MainWindow()
{
}
