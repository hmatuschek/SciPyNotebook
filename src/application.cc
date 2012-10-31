#include "application.hh"
#include <QMessageBox>


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
Application::onQuit()
{
  // check if any notebook is modified:
  bool modified = false;
  for (QList<Notebook *>::iterator nb=_notebooks.begin(); nb!=_notebooks.end(); nb++) {
    modified |= (*nb)->isModified();
  }

  // If any notebook is modified, ask user:
  if (modified) {
    if (QMessageBox::Yes != QMessageBox::question(0, tr("Discard changes"),
                                                  tr("One or more notebooks have unsaved changes, "
                                                     "do you want to close SciPy Notebook anyway?"),
                                                  QMessageBox::Yes, QMessageBox::No))
    {
      return;
    }
  }

  QApplication::quit();
}

