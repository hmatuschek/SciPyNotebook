#include "cell.hh"
#include "cellstatus.hh"
#include "pythonengine.hh"

#include <iostream>



Cell::Cell(QWidget *parent) :
    QFrame(parent)
{
    // Init the layout
    QHBoxLayout *hbox = new QHBoxLayout();
    this->cell_status = new CellStatus(this);
    hbox->addWidget(this->cell_status);
    hbox->setSpacing(0);
    hbox->setContentsMargins(0,3,0,0);
    this->cellbox = new QVBoxLayout();
    this->cellbox->setContentsMargins(0,0,0,0);
    this->cellbox->setSizeConstraint(QLayout::SetMinimumSize);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->cellbox->setSpacing(0);
    hbox->addLayout(this->cellbox);

    // Instantiate empty code cell
    this->codecell = new CodeCell(this);
    this->cellbox->addWidget(this->codecell);

    // Instantiate result cell
    this->resultcell = new ResultCell();
    this->cellbox->addWidget(this->resultcell);
    this->resultcell->setVisible(false);

    // Eye candy
    this->setLayout(hbox);
}


void
Cell::evaluate()
{
    // Get execution engine
    PythonEngine *engine = PythonEngine::get();

    // Get code as ASCII
    QString code = this->codecell->document()->toPlainText();

    // signal cell evaluation
    this->cell_status->setStatusRunning();

    // Redirect stdout and stderr:
    engine->setStdout(this->resultcell->getStdoutStream());
    engine->setStderr(this->resultcell->getStderrStream());

    // Clear and hide result cell:
    this->resultcell->document()->clear();
    this->resultcell->setVisible(false);

    // Evaluate code:
    /// \todo Make sure this runs in a separate thread
    PyObject *result = 0;
    if (0 == (result = PyRun_String(code.toStdString().c_str(), Py_file_input,
                                    engine->getGlobals(), engine->getLocals())))
    {
        // Change color of cell status bar
        this->cell_status->setStatusError();

        // Show result cell
        this->resultcell->setVisible(true);

        // print traceback to sys.stderr
        PyErr_Print();

        return;
    }

    this->cell_status->setStatusSuccess();

    Py_DECREF(result);
}


void
Cell::setFocus()
{
    this->codecell->setFocus();
}