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
    QObject::connect(this->cell_status, SIGNAL(clicked()), this, SLOT(onStatusClicked()));

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

    // Set set codecell to be focus-proxy of the cell
    this->setFocusProxy(this->codecell);

    // Instantiate result cell
    this->resultcell = new ResultCell(this);
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

    // Clear line marks in codecell:
    this->codecell->clearLineMarks();

    // Redirect stdout and stderr:
    engine->setStdout(this->resultcell->getStdoutStream());
    engine->setStderr(this->resultcell->getStderrStream());

    // Clear and hide result cell:
    this->resultcell->document()->clear();
    this->resultcell->setVisible(false);

    // Precompile code:
    PyObject *prec_code = 0;
    if (0 == (prec_code = Py_CompileString(code.toStdString().c_str(), "<cell>",
                                           Py_file_input)))
    {
        // Change color of cell status bar:
        this->cell_status->setStatusError();

        // Extract Exception:
        PyObject *exec, *pvalue, *traceback;
        PyErr_Fetch(&exec, &pvalue, &traceback);

        if (0 == pvalue)
        {
          // oops:
          qWarning("Compilation failed but no exception found.");
          return;
        }

        // If there is a traceback:
        if (0 != traceback)
        {
          // Extract the line number of the first frame and mark the line in codecell
          PyObject *lineno = PyObject_GetAttrString(traceback, "tb_lineno");
          this->codecell->markLine(PyInt_AsSsize_t(lineno));
          Py_DECREF(lineno);
        }

        // Restore exception to be printed:
        PyErr_Restore(exec, pvalue, traceback);

        // Show exception:
        this->resultcell->setVisible(true);

        PyErr_Print();

        // Done.
        return;
    }

    // Evaluate code:
    /// \todo Make sure this runs in a separate thread
    PyObject *result = 0;
    if (0 == (result = PyEval_EvalCode((PyCodeObject *)prec_code,
                                       ctx->getGlobals(), ctx->getGlobals())))
    {
        // Change color of cell status bar
        this->cell_status->setStatusError();

        // Extract Exception:
        PyObject *exec, *pvalue, *traceback;
        PyErr_Fetch(&exec, &pvalue, &traceback);

        if (0 == pvalue)
        {
          // oops:
          qWarning("Compilation failed but no exception found.");
          return;
        }

        // If there is a traceback:
        if (0 != traceback)
        {
          // Extract the line number of the first frame and mark the line in codecell
          PyObject *lineno = PyObject_GetAttrString(traceback, "tb_lineno");
          this->codecell->markLine(PyInt_AsSsize_t(lineno));
          Py_DECREF(lineno);
        }

        // Restore exception to be printed:
        PyErr_Restore(exec, pvalue, traceback);

        // Show result cell
        this->resultcell->setVisible(true);

        // print traceback to sys.stderr
        PyErr_Print();

        return;
    }

    this->cell_status->setStatusSuccess();

    Py_DECREF(result);
    Py_DECREF(prec_code);
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


void
Cell::onStatusClicked()
{
  emit this->statusClicked();
}
