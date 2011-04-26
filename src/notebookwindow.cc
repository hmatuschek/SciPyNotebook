#include "notebookwindow.hh"

#include <QFile>
#include <QFileDialog>
#include <QMenuBar>



NotebookWindow::NotebookWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->initNotebookWindow(new Notebook());
}


NotebookWindow::NotebookWindow(const QString &filename, QWidget *parent)
    : QMainWindow(parent)
{
    this->initNotebookWindow(new Notebook(this, filename));
}


void
NotebookWindow::initNotebookWindow(Notebook *notebook)
{

    // Set window title
    this->setWindowTitle("SciPy Notebook");
    this->resize(533, 640);

    // Store notebook widget
    this->notebook = notebook;
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
    QObject::connect(openAct, SIGNAL(triggered()), this, SLOT(openSlot()));

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
    newNotebook->show();
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
}

