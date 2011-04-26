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
    QAction *closeAct;
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

    QMenu *cellMenu;
    QAction *newCellAct;
    QAction *delCellAct;
    QAction *splitCellAct;
    QAction *joinCellsAct;
    QAction *evalCellAct;

    QMenu *helpMenu;
    QAction *aboutAct;


public:
    /**
     * Constructs a new notebook window with an empty notebook.
     */
    explicit NotebookWindow(QWidget *parent = 0);

    NotebookWindow(const QString &filename, QWidget *parent);


protected:
    void initNotebookWindow(Notebook *notebook);


public slots:
    void newSlot();
    void openSlot();
    void closeSlot();
    void saveSlot();
    void saveAsSlot();
    void quitSlot();

    void aboutSlot();
};

#endif // NOTEBOOKWINDOW_HH
