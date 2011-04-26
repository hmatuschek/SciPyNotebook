#ifndef NOTEBOOKWINDOW_HH
#define NOTEBOOKWINDOW_HH

#include "notebook.hh"

#include <QMainWindow>
#include <QMenu>
#include <QAction>



class NotebookWindow : public QMainWindow
{
    Q_OBJECT

protected:
    Notebook *notebook;

    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *printAct;
    QAction *quitAct;

    QMenu *editMenu;
    QAction *undoAct;
    QAction *redoAct;
    QAction *copyAct;
    QAction *cutAct;
    QAction *pasteAct;
    QAction *prefAct;


public:
    /**
     * Constructs a new notebook window with an empty notebook.
     */
    explicit NotebookWindow(QWidget *parent = 0);

    NotebookWindow(const QString &filename, QWidget *parent);


protected:
    void initNotebookWindow(Notebook *notebook);


public slots:
    void openSlot();
    void saveSlot();
    void saveAsSlot();

};

#endif // NOTEBOOKWINDOW_HH
