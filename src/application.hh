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


class Application : public QApplication
{
  Q_OBJECT

public:
  explicit Application(int & argc, char ** argv);
  
  inline QAction *newNotebookAction() { return _newAct; }
  inline QAction *openFileAction() { return _openAct; }
  inline QAction *quitAction() { return _quitAct; }
  inline QAction *showPreferencesAction() { return _preferencesAct; }
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
