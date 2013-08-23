/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011, 2012 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#include "notebookwindow.hh"
#include "aboutdialog.hh"
#include "preferences.hh"
#include "preferencesdialog.hh"
#include "application.hh"

#include <QFile>
#include <QFileDialog>
#include <QMenuBar>
#include <QApplication>
#include <QMessageBox>

#include "logo.hh"


NotebookWindow::NotebookWindow(QWidget *parent) :
    QMainWindow(parent)
{
  _notebook_view = new NotebookView();
  _notebook_view->setNotebook(new Notebook(this));

  QObject::connect(_notebook_view->notebook(), SIGNAL(modifiedStateChanged()),
                   this, SLOT(updateWindowTitle()));

  _initNotebookWindow();
}


NotebookWindow::NotebookWindow(const QString &filename, QWidget *parent)
    : QMainWindow(parent)
{
  _notebook_view = new NotebookView();
  _notebook_view->setNotebook(new Notebook(filename, this));
  QObject::connect(_notebook_view->notebook(), SIGNAL(modifiedStateChanged()),
                   this, SLOT(updateWindowTitle()));

  _initNotebookWindow();
}


NotebookWindow::~NotebookWindow() {
  // Pass...
}


Notebook *
NotebookWindow::notebook() {
  return _notebook_view->notebook();
}

void
NotebookWindow::_initNotebookWindow()
{
  // Set window title
  updateWindowTitle();
  setWindowIcon(QIcon(QPixmap(logo_xpm)));

  // Set window size...
  resize(600, 640);

  // Store notebook widget into a scrollarea
  _scrolledWindow = new QScrollArea();

  // Layout notebook
  _scrolledWindow->setWidget(_notebook_view);
  _scrolledWindow->setWidgetResizable(true);
  setCentralWidget(_scrolledWindow);

  // Create actions:
  QAction *saveAction = new QAction(tr("Save"), this);
  saveAction->setShortcut(QKeySequence::Save);
  QAction *saveAsAction = new QAction(tr("Save"), this);
  saveAsAction->setShortcut(QKeySequence::SaveAs);

  QAction *undoAction = new QAction(tr("Undo"), this);
  undoAction->setShortcut(QKeySequence::Undo);
  QAction *redoAction = new QAction(tr("Redo"), this);
  redoAction->setShortcut(QKeySequence::Redo);
  QAction *copyAction = new QAction(tr("Copy"), this);
  copyAction->setShortcut(QKeySequence::Copy);
  QAction *cutAction = new QAction(tr("Cut"), this);
  cutAction->setShortcut(QKeySequence::Cut);
  QAction *pasteAction = new QAction(tr("Paste"), this);
  pasteAction->setShortcut(QKeySequence::Paste);

  QAction *newCellAction = new QAction(tr("New cell"), this);
  newCellAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
  QAction *delCellAction = new QAction(tr("Delete cell"), this);
  delCellAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Backspace));
  QAction *splitCellAction = new QAction(tr("Split cell"), this);
  splitCellAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Space));
  QAction *joinCellAction = new QAction(tr("Join cells"), this);
  joinCellAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Space));
  QAction *evalCellAction = new QAction(tr("Evaluate cell"), this);
  evalCellAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
  QAction *evalAllCellsAction = new QAction(tr("Evaluate all cells"), this);
  evalAllCellsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Return));

  QAction *closeAction = new QAction(tr("Close Notebook"), this);
  closeAction->setShortcut(QKeySequence::Close);

  QObject::connect(saveAction, SIGNAL(triggered()), _notebook_view->notebook(), SLOT(save()));
  QObject::connect(saveAsAction, SIGNAL(triggered()), _notebook_view->notebook(), SLOT(saveAs()));

  QObject::connect(newCellAction, SIGNAL(triggered()), _notebook_view, SLOT(newCell()));
  QObject::connect(delCellAction, SIGNAL(triggered()), _notebook_view, SLOT(deleteCell()));
  QObject::connect(splitCellAction, SIGNAL(triggered()), _notebook_view, SLOT(splitCell()));
  QObject::connect(joinCellAction, SIGNAL(triggered()), _notebook_view, SLOT(joinCell()));
  QObject::connect(evalCellAction, SIGNAL(triggered()), _notebook_view, SLOT(evalCell()));
  QObject::connect(evalAllCellsAction, SIGNAL(triggered()), _notebook_view->notebook(), SLOT(evalAllCells()));
  QObject::connect(closeAction, SIGNAL(triggered()), this, SLOT(onCloseNotebook()));

  QObject::connect(undoAction, SIGNAL(triggered()), _notebook_view, SLOT(undo()));
  QObject::connect(redoAction, SIGNAL(triggered()), _notebook_view, SLOT(redo()));
  QObject::connect(copyAction, SIGNAL(triggered()), _notebook_view, SLOT(copy()));
  QObject::connect(cutAction, SIGNAL(triggered()), _notebook_view, SLOT(cut()));
  QObject::connect(pasteAction, SIGNAL(triggered()), _notebook_view, SLOT(paste()));

  // Assemble menu:
  Application *application = static_cast<Application *>(QApplication::instance());

  _fileMenu = menuBar()->addMenu(tr("&File"));
  _fileMenu->addAction(application->newNotebookAction());
  _fileMenu->addAction(application->openFileAction());
  _recentMenu = _fileMenu->addMenu(tr("Recent Files"));
  _fileMenu->addSeparator();
  _fileMenu->addAction(saveAction);
  _fileMenu->addAction(saveAsAction);
  _fileMenu->addSeparator();
  //_fileMenu->addAction(_notebook_view->notebook()->printNotebookAction());
  _fileMenu->addSeparator();
  _fileMenu->addAction(application->quitAction());

  // Assemble "recent" menu:

  _editMenu = menuBar()->addMenu(tr("&Edit"));
  _editMenu->addAction(undoAction);
  _editMenu->addAction(redoAction);
  _editMenu->addSeparator();
  _editMenu->addAction(copyAction);
  _editMenu->addAction(cutAction);
  _editMenu->addAction(pasteAction);
  _editMenu->addSeparator();
  _editMenu->addAction(application->showPreferencesAction());

  _cellMenu = menuBar()->addMenu(tr("&Notebook"));
  _cellMenu->addAction(newCellAction);
  _cellMenu->addAction(delCellAction);
  _cellMenu->addAction(splitCellAction);
  _cellMenu->addAction(joinCellAction);
  _cellMenu->addSeparator();
  _cellMenu->addAction(evalCellAction);
  _cellMenu->addAction(evalAllCellsAction);
  _cellMenu->addSeparator();
  _cellMenu->addAction(closeAction);

  _helpMenu = menuBar()->addMenu(tr("&Help"));
  _helpMenu->addAction(application->showAboutAction());
}


void
NotebookWindow::updateWindowTitle() {
  QString title;
  if (_notebook_view->notebook()->hasFileName()) {
    title = "SciPy Notebook - " + _notebook_view->notebook()->fileName();
  } else {
    title = "SciPy Notebook - New File";
  }

  if (_notebook_view->notebook()->isModified()) {
    title.append("*");
  }
  setWindowTitle(title);
}


void
NotebookWindow::onCloseNotebook() {
  if (notebook()->isModified()) {
    if (QMessageBox::Yes != QMessageBox::question(0, tr("Discard changes"),
                                                  tr("This notbook has unsaved changes, do you "
                                                     "want to close it anyway?"),
                                                  QMessageBox::Yes, QMessageBox::No))
    {
      return;
    }
  }

  close();
}
