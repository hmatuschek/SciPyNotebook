/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#include "cell.hh"
#include "cellstatus.hh"
#include "pythonengine.hh"
#include "pythoncontext.hh"


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
Cell::evaluate(PythonContext *ctx)
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
                                    ctx->getGlobals(), ctx->getLocals())))
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


void
Cell::serializeCode(QIODevice &device)
{
    device.write(this->codecell->document()->toPlainText().toAscii());
}


void
Cell::setCode(const QString &code)
{
  this->codecell->document()->setPlainText(code);
  this->codecell->document()->adjustSize();
}


void
Cell::undoSlot()
{
  this->codecell->undo();
}


void
Cell::redoSlot()
{
  this->codecell->redo();
}
