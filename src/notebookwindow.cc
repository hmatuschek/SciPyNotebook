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


  // Assemble menu:
  Application *application = static_cast<Application *>(QApplication::instance());

  _fileMenu = menuBar()->addMenu(tr("&File"));
  _fileMenu->addAction(application->newNotebookAction());
  _fileMenu->addAction(application->openFileAction());
  _fileMenu->addSeparator();
  _fileMenu->addAction(_notebook_view->notebook()->saveNotebookAction());
  _fileMenu->addAction(_notebook_view->notebook()->saveNotebookAsAction());
  _fileMenu->addSeparator();
  _fileMenu->addAction(_notebook_view->notebook()->printNotebookAction());
  _fileMenu->addSeparator();
  _fileMenu->addAction(application->quitAction());

  _editMenu = this->menuBar()->addMenu(tr("&Edit"));
  _editMenu->addAction(_notebook_view->notebook()->undoAction());
  _editMenu->addAction(_notebook_view->notebook()->redoAction());
  _editMenu->addSeparator();
  _editMenu->addAction(_notebook_view->notebook()->copyAction());
  _editMenu->addAction(_notebook_view->notebook()->cutAction());
  _editMenu->addAction(_notebook_view->notebook()->pasteAction());
  _editMenu->addSeparator();
  _editMenu->addAction(application->showPreferencesAction());

  _cellMenu = this->menuBar()->addMenu(tr("&Notebook"));
  _cellMenu->addAction(_notebook_view->notebook()->newCellAction());
  _cellMenu->addAction(_notebook_view->notebook()->deleteCellAction());
  _cellMenu->addAction(_notebook_view->notebook()->splitCellAction());
  _cellMenu->addAction(_notebook_view->notebook()->joinCellsAction());
  _cellMenu->addSeparator();
  _cellMenu->addAction(_notebook_view->notebook()->evalCellAction());
  _cellMenu->addAction(_notebook_view->notebook()->evalAllCellsAction());
  _cellMenu->addSeparator();
  QAction *closeAction = new QAction(tr("Close Notebook"), this);
  closeAction->setShortcut(QKeySequence::Close);
  QObject::connect(closeAction, SIGNAL(triggered()), this, SLOT(onCloseNotebook()));
  _cellMenu->addAction(closeAction);

  _helpMenu = this->menuBar()->addMenu(tr("&Help"));
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
