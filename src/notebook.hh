#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QScrollArea>
#include <QLinkedList>
#include <QLayout>

#include "cell.hh"


class Notebook : public QFrame
{
    Q_OBJECT

protected:
    QLinkedList<Cell *> _cells;
    QShortcut *_eval_shortcut;
    QShortcut *_new_cell_shortcut;
    QBoxLayout *cell_layout;
public:
    explicit Notebook(QWidget *parent = 0);


signals:


public slots:
    void onEvalCell();
    void onNewCell();
};

#endif // NOTEBOOK_H
