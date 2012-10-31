#ifndef NOTEBOOKWINDOW_HH
#define NOTEBOOKWINDOW_HH

#include "notebook.hh"

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "notebookview.hh"


class NotebookWindow : public QMainWindow
{
  Q_OBJECT

protected:
  NotebookView *_notebook_view;
  QScrollArea *_scrolledWindow;

  QMenu *_fileMenu;
  QMenu *_editMenu;
  QMenu *_cellMenu;
  QMenu *_helpMenu;


public:
  /** Constructs a new notebook window with an empty notebook. */
  explicit NotebookWindow(QWidget *parent = 0);
  NotebookWindow(const QString &filename, QWidget *parent=0);
  virtual ~NotebookWindow();

protected:
  void _initNotebookWindow();
};

#endif // NOTEBOOKWINDOW_HH
