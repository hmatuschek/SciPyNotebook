#include "notebookwindow.hh"
#include "aboutdialog.hh"
#include "preferences.hh"
#include "preferencesdialog.hh"


#include <QFile>
#include <QFileDialog>
#include <QMenuBar>
#include <QApplication>

#include "logo.hh"


NotebookWindow::NotebookWindow(QWidget *parent) :
    QMainWindow(parent)
{
  this->initNotebookWindow(new Notebook());
}


NotebookWindow::NotebookWindow(const QString &filename, QWidget *parent)
    : QMainWindow(parent)
{
  this->initNotebookWindow(new Notebook(filename, 0));
}


void
NotebookWindow::initNotebookWindow(Notebook *notebook)
{

  // Set window title
  if (notebook->hasFileName())
    this->setWindowTitle("SciPy Notebook - " + notebook->fileName());
  else
    this->setWindowTitle("SciPy Notebook - New File*");

  this->setWindowIcon(QIcon(QPixmap(logo_xpm)));

  // Set window size...
  this->resize(600, 640);

  // Store notebook widget into a scrollarea
  this->notebook = notebook;
  this->scrolledWindow = new QScrollArea();

  // Connect to signals:
  QObject::connect(this->notebook, SIGNAL(makeVisible(QPoint)),
                   this, SLOT(makeVisible(QPoint)));
  QObject::connect(this->notebook, SIGNAL(modified()),
                   this, SLOT(notebookModified()));
  QObject::connect(this->notebook, SIGNAL(saved()),
                   this, SLOT(notebookSaved()));

  // Layout notebook
  this->scrolledWindow->setWidget(this->notebook);
  this->scrolledWindow->setWidgetResizable(true);
  this->setCentralWidget(this->scrolledWindow);

  // Assemble actions:
  newAct = new QAction(tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("A new, empty notebook."));
  QObject::connect(newAct, SIGNAL(triggered()), this, SLOT(newSlot()));

  openAct = new QAction(tr("&Open"), this);
  openAct->setShortcut(QKeySequence::Open);
  openAct->setStatusTip(tr("Open a notebook or python script."));
  QObject::connect(openAct, SIGNAL(triggered()), this, SLOT(openSlot()));

  closeAct = new QAction(tr("&Close"), this);
  closeAct->setShortcut(QKeySequence::Close);
  closeAct->setStatusTip(tr("Closes the current notebook."));
  QObject::connect(closeAct, SIGNAL(triggered()), this, SLOT(closeSlot()));

  saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcut(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the notebook."));
  QObject::connect(saveAct, SIGNAL(triggered()), this, SLOT(saveSlot()));

  saveAsAct = new QAction(tr("Save &As"), this);
  saveAsAct->setShortcut(QKeySequence::SaveAs);
  saveAsAct->setStatusTip(tr("Save the notbook under a different name."));
  QObject::connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsSlot()));

  printAct = new QAction(tr("&Print"), this);
  printAct->setShortcut(QKeySequence::Print);
  printAct->setStatusTip(tr("Print the notebook."));

  quitAct = new QAction(tr("&Quit"), this);
  quitAct->setShortcut(QKeySequence::Quit);
  quitAct->setStatusTip(tr("Quit SciPy Notebook."));
  QObject::connect(quitAct, SIGNAL(triggered()), this, SLOT(quitSlot()));

  undoAct = new QAction(tr("Undo"), this);
  undoAct->setShortcut(QKeySequence::Undo);
  undoAct->setStatusTip(tr("Undo last changes."));
  QObject::connect(undoAct, SIGNAL(triggered()), this->notebook, SLOT(undoSlot()));

  redoAct = new QAction(tr("Redo"), this);
  redoAct->setShortcut(QKeySequence::Redo);
  redoAct->setStatusTip(tr("Redo last changes."));
  QObject::connect(redoAct, SIGNAL(triggered()), this->notebook, SLOT(redoSlot()));

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
  QObject::connect(prefAct, SIGNAL(triggered()), this, SLOT(preferencesSlot()));

  newCellAct = new QAction(tr("New Cell"), this);
  newCellAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
  newCellAct->setStatusTip(tr("Creates a new cell bayond the curtrent one."));
  QObject::connect(newCellAct, SIGNAL(triggered()), this->notebook, SLOT(onNewCell()));

  delCellAct = new QAction(tr("Delete cell"), this);
  delCellAct->setShortcut(Qt::CTRL + Qt::Key_Backspace);
  delCellAct->setStatusTip(tr("Deletes the current cell."));
  QObject::connect(delCellAct, SIGNAL(triggered()), this->notebook, SLOT(delCellSlot()));

  splitCellAct = new QAction(tr("Split Cell"), this);
  splitCellAct->setShortcut(Qt::CTRL + Qt::Key_Space);
  splitCellAct->setStatusTip(tr("Splits current cell into two."));
  QObject::connect(splitCellAct, SIGNAL(triggered()), this->notebook, SLOT(splitCellSlot()));

  joinCellsAct = new QAction(tr("Join Cells"), this);
  joinCellsAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Space);
  joinCellsAct->setStatusTip(tr("Joins the current cell with the next one."));
  QObject::connect(joinCellsAct, SIGNAL(triggered()), this->notebook, SLOT(joinCellsSlot()));

  evalCellAct = new QAction(tr("Evaluate Cell"), this);
  evalCellAct->setShortcut(Qt::CTRL + Qt::Key_Return);
  evalCellAct->setStatusTip(tr("Evaluates the current cell."));
  QObject::connect(evalCellAct, SIGNAL(triggered()), this->notebook, SLOT(evalCellSlot()));

  runAllAct = new QAction(tr("Evaluate all"), this);
  runAllAct->setShortcut(Qt::CTRL + Qt::Key_R);
  runAllAct->setStatusTip(tr("Evaluates all cells of the notebook."));
  QObject::connect(runAllAct, SIGNAL(triggered()), this->notebook, SLOT(evalAllCellsSlot()));

  aboutAct = new QAction(tr("About SciPy Notebook"), this);
  aboutAct->setStatusTip(tr("Shows some information about SciPy Notebook."));
  QObject::connect(this->aboutAct, SIGNAL(triggered()), this, SLOT(aboutSlot()));


  // Assemble menu:
  fileMenu = this->menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(this->newAct);
  fileMenu->addAction(this->openAct);
  fileMenu->addAction(this->closeAct);
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

  cellMenu = this->menuBar()->addMenu(tr("&Notebook"));
  cellMenu->addAction(this->newCellAct);
  cellMenu->addAction(this->delCellAct);
  cellMenu->addAction(this->splitCellAct);
  cellMenu->addAction(this->joinCellsAct);
  cellMenu->addSeparator();
  cellMenu->addAction(this->evalCellAct);
  cellMenu->addAction(this->runAllAct);

  helpMenu = this->menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(this->aboutAct);
}


void
NotebookWindow::newSlot()
{
  NotebookWindow *w = new NotebookWindow();
  /// \todo Register notebook window to application!
  w->show();
}


void
NotebookWindow::openSlot()
{
    // Assemble Save as... dialog
    QFileDialog dialog(this);
    /// \todo Select directory properly.
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("Python Scripts (*.py)"));

    if (! dialog.exec()) {
        // Abort...
        return;
    }

    // Get filename and save it:
    QString file = dialog.selectedFiles().front();
    NotebookWindow *newNotebook = new NotebookWindow(file, 0);
    /// \todo Register notebook window to application!
    newNotebook->show();
}


void
NotebookWindow::closeSlot()
{
  /// \todo Check if notebook is unsaved!
  this->close();
}


void
NotebookWindow::saveSlot()
{
    if(! this->notebook->hasFileName())
    {
        this->saveAsSlot();
        return;
    }

    this->notebook->save();
}


void
NotebookWindow::saveAsSlot()
{
    // Assemble Save as... dialog
    QFileDialog dialog(this);
    /// \todo Select directory properly.
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Python Scripts (*.py)"));

    if (! dialog.exec()) {
        // Abort...
        return;
    }

    // Get filename and save it:
    QString file = dialog.selectedFiles().front();
    this->notebook->setFileName(file);
    this->notebook->save();

    this->setWindowTitle("SciPy Notebook - " + file);
}


void
NotebookWindow::quitSlot()
{
  // Check if any notebook is not saved yet.
  QApplication::quit();
}


void
NotebookWindow::aboutSlot()
{
  AboutDialog dialog;
  dialog.exec();
}


void
NotebookWindow::preferencesSlot()
{
  PreferencesDialog dialog;

  if(! dialog.exec())
  {
    // Abort...
    return;
  }

  // Get preferences from fialog and store it in preferences instance
  Preferences *preferences = Preferences::get();
  preferences->setFont(dialog.font());
  preferences->setTabSize(dialog.tabSize());
  preferences->setPreamble(dialog.preamble());
  preferences->save();
}


void
NotebookWindow::makeVisible(QPoint coord)
{
  this->scrolledWindow->ensureVisible(coord.x(), coord.y(), 50, 50);
}


void
NotebookWindow::notebookModified()
{
  if (notebook->hasFileName())
    this->setWindowTitle("SciPy Notebook - " + notebook->fileName() + "*");
}

void
NotebookWindow::notebookSaved()
{
  this->setWindowTitle("SciPy Notebook - " + notebook->fileName());
}

