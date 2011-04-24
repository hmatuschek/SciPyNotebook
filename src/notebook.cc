#include "notebook.hh"
#include <QApplication>


Notebook::Notebook(QWidget *parent) :
    QFrame(parent)
{
    this->cell_layout = new QVBoxLayout();
    this->cell_layout->setSpacing(0);
    this->cell_layout->setContentsMargins(0,0,0,0);
    this->setLayout(this->cell_layout);
    this->cell_layout->setSizeConstraint(QLayout::SetMinimumSize);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    // Register shortcut for evaluation of current cell
    this->_eval_shortcut = new QShortcut(Qt::CTRL + Qt::Key_Return, this);
    QObject::connect(this->_eval_shortcut, SIGNAL(activated()),
                     this, SLOT(onEvalCell()));

    // Register shortcut for evaluation of current cell
    this->_new_cell_shortcut = new QShortcut(Qt::CTRL + Qt::Key_N, this);
    QObject::connect(this->_new_cell_shortcut, SIGNAL(activated()),
                     this, SLOT(onNewCell()));

    // Create an empty cell
    Cell *new_cell = new Cell(this);
    new_cell->setFocus();
    this->cell_layout->addWidget(new_cell);
}



void
Notebook::onEvalCell()
{
    //Eval cell
    Cell *cell = (Cell *)(QApplication::focusWidget()->parent());

    // Check if w is a Cell object and if it is associated with this notebook.
    if(-1 == this->layout()->indexOf(cell))
        return;

    cell->evaluate();
}


void
Notebook::onNewCell()
{
    // Get the current cell and its index (if known)
    QWidget *w = (QWidget *)(QApplication::focusWidget()->parent());
    int index = this->layout()->indexOf(w);
    if(0 <= index)
        index++;

    //Append a new cell
    Cell *new_cell = new Cell();
    new_cell->setFocus();

    // Insert
    this->cell_layout->insertWidget(index, new_cell);
}

