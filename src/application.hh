/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011,2012 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#ifndef APPLICATION_HH
#define APPLICATION_HH

#include <QApplication>
#include <QAction>

#include "notebook.hh"
#include "cell.hh"


/** Central class implementing application global menu actions. */
class Application : public QApplication
{
  Q_OBJECT

public:
  /** Constructs the application. */
  explicit Application(int & argc, char ** argv);
  
  /** Returns a weak reference to the "new notebook" action. */
  inline QAction *newNotebookAction() { return _newAct; }
  /** Returns a weak reference to the "open file" action. */
  inline QAction *openFileAction() { return _openAct; }
  /** Returns a weak reference to the "quit" action. */
  inline QAction *quitAction() { return _quitAct; }
  /** Returns a weak reference to the "preferences" action. */
  inline QAction *showPreferencesAction() { return _preferencesAct; }
  /** Returns a weak reference to the "about" action. */
  inline QAction *showAboutAction() { return _aboutAct; }

  void addNotebook(Notebook *notebook);

private slots:
  void onQuit();
  void onNewNotebook();
  void onOpenNotebook();
  void onNotebookDelete(QObject *notebook);
  void onShowAbout();
  void onShowPreferences();

private:
  QList<Notebook *> _notebooks;

  QAction *_newAct;
  QAction *_openAct;
  QAction *_quitAct;
  QAction *_preferencesAct;
  QAction *_aboutAct;

};

#endif // APPLICATION_HH
