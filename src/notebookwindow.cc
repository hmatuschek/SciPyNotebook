#include "notebookwindow.hh"
#include "aboutdialog.hh"
#include "preferences.hh"
#include "preferencesdialog.hh"
#include "application.hh"

#include <QFile>
#include <QFileDialog>
#include <QMenuBar>
#include <QApplication>

#include "logo.hh"


NotebookWindow::NotebookWindow(QWidget *parent) :
    QMainWindow(parent)
{
  _notebook_view = new NotebookView();
  _notebook_view->setNotebook(new Notebook(this));

  _initNotebookWindow();
}


NotebookWindow::NotebookWindow(const QString &filename, QWidget *parent)
    : QMainWindow(parent)
{
  _notebook_view = new NotebookView();
  _notebook_view->setNotebook(new Notebook(filename, this));

  _initNotebookWindow();
}


NotebookWindow::~NotebookWindow() {
  // Pass...
}


void
NotebookWindow::_initNotebookWindow()
{
  // Set window title
  if (_notebook_view->notebook()->hasFileName())
    setWindowTitle("SciPy Notebook - " + _notebook_view->notebook()->fileName());
  else
    setWindowTitle("SciPy Notebook - New File*");
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
  /*_editMenu->addAction(COPY);
  _editMenu->addAction(CUT);
  _editMenu->addAction(PASTE);*/
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

  _helpMenu = this->menuBar()->addMenu(tr("&Help"));
  _helpMenu->addAction(application->showAboutAction());
}


