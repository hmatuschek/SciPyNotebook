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

#include "application.hh"
#include <QMessageBox>
#include <QFileDialog>
#include "notebookwindow.hh"
#include "aboutdialog.hh"
#include "preferencesdialog.hh"
#include "preferences.hh"


Application::Application(int &argc, char **argv) :
  QApplication(argc, argv)
{
  // Assemble actions:
  _newAct = new QAction(tr("&New"), this);
  _newAct->setShortcuts(QKeySequence::New);
  _newAct->setStatusTip(tr("A new, empty notebook."));

  _openAct = new QAction(tr("&Open"), this);
  _openAct->setShortcut(QKeySequence::Open);
  _openAct->setStatusTip(tr("Open a notebook or python script."));

  _quitAct = new QAction(tr("&Quit"), this);
  _quitAct->setShortcut(QKeySequence::Quit);
  _quitAct->setStatusTip(tr("Quit SciPy Notebook."));

  _preferencesAct = new QAction(tr("Preferences"), this);
  _preferencesAct->setShortcut(QKeySequence::Preferences);

  _aboutAct = new QAction(tr("About SciPy Notebook"), this);
  _aboutAct->setStatusTip(tr("Shows some information about SciPy Notebook."));

  QObject::connect(_quitAct, SIGNAL(triggered()), this, SLOT(onQuit()));
  QObject::connect(_newAct, SIGNAL(triggered()), this, SLOT(onNewNotebook()));
  QObject::connect(_openAct, SIGNAL(triggered()), this, SLOT(onOpenNotebook()));
  QObject::connect(_aboutAct, SIGNAL(triggered()), this, SLOT(onShowAbout()));
  QObject::connect(_preferencesAct, SIGNAL(triggered()), this, SLOT(onShowPreferences()));
}


void
Application::addNotebook(Notebook *notebook) {
  _notebooks.append(notebook);
  QObject::connect(notebook, SIGNAL(destroyed(QObject *)), this, SLOT(onNotebookDelete(QObject*)));
}


void
Application::onNotebookDelete(QObject *notebook) {
  _notebooks.removeAll(static_cast<Notebook *>(notebook));
}


void
Application::onNewNotebook() {
  NotebookWindow *window = new NotebookWindow();
  window->show();
}


void
Application::onOpenNotebook() {
  QString filename = QFileDialog::getOpenFileName(
        0, tr("Open file..."), "", tr("Python Source File (*.py)"));
  if (0 == filename.size()) { return; }

  // Save filename in "recent files":
  Preferences::get()->addRecentFile(filename);
  // open...
  NotebookWindow *window = new NotebookWindow(filename);
  window->show();
}


void
Application::onQuit()
{
  // check if any notebook is modified:
  bool modified = false;
  for (QList<Notebook *>::iterator nb=_notebooks.begin(); nb!=_notebooks.end(); nb++) {
    modified |= (*nb)->isModified();
  }

  // If any notebook is modified, ask user:
  if (modified) {
    if (QMessageBox::Yes != QMessageBox::question(
          0, tr("Discard changes"),
          tr("One or more notebooks have unsaved changes, "
             "do you want to close SciPy Notebook anyway?"),
          QMessageBox::Yes, QMessageBox::No)) { return; }
  }

  QApplication::quit();
}


void
Application::onShowAbout() {
  AboutDialog dialog; dialog.exec();
}

void
Application::onShowPreferences() {
  PreferencesDialog dialog; dialog.exec();
}
