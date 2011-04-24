#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>

#include "notebook.hh"


class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    /**
     * Holds the reference to the console widget.
     */
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
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
};

#endif // MAINWINDOW_H
