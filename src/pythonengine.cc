/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011, 2012 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#include <Python.h>
#include "pythonengine.hh"
#include "streamwrapper.hh"
#include "cell.hh"
#include <iostream>


PythonEngine *PythonEngine::_instance = 0;


PythonEngine::PythonEngine(QObject *parent)
  : QThread(parent), _isrunning(true)
{
  Py_Initialize();

  PythonEngine::_instance = this;

  // Register SciPyNotebookStreamWrapper class:
  if (0 > SciPyNotebookStreamWrapperType_init()) {
    exit(-1);
  }
}



PythonEngine::~PythonEngine()
{
    Py_Finalize();
    PythonEngine::_instance = 0;
}


PythonEngine *
PythonEngine::get()
{
  if (0 != PythonEngine::_instance) {
    return PythonEngine::_instance;
  }

  // Create engine...
  PythonEngine::_instance = new PythonEngine();
  // Start engine...
  PythonEngine::_instance->start();

  return PythonEngine::_instance;
}


void
PythonEngine::queueCell(Cell *cell) {
  _mutex.lock();
  cell->setEvaluationState(Cell::QUEUED);
  _queue.append(cell);
  _notEmptyCondition.wakeAll();
  _mutex.unlock();
}


void
PythonEngine::run()
{
  // Tell Python about this thread...
  PyGILState_STATE gstate;
  gstate = PyGILState_Ensure();

  // Get global sys module:
  PyObject *sys_module = 0;
  if (0 == (sys_module = PyImport_AddModule("sys"))) {
    exit(-1);
  }
  PyObject *sys_dict = PyModule_GetDict(sys_module);

  // This will hold the current cell being executed:
  Cell *current_cell = 0;

  // Loop...
  while (_isrunning) {
    _mutex.lock();
    // Wait for new cells if queue is empty:
    if (0 == _queue.size()) { _notEmptyCondition.wait(&_mutex); }
    // If there was no cell in the queue -> retry
    if (0 == _queue.size()) { continue; }
    // Take next cell from queue
    current_cell = _queue.front(); _queue.pop_front();
    _mutex.unlock();

    // Update state of cell
    current_cell->setEvaluationState(Cell::EVALUATING);

    // Redirect stderr & stdout streams:
#ifdef PY_MAJOR_VERSION >= 3
    PyDict_SetItem(sys_dict, PyBytes_FromString("stdout"),
                   SciPyNotebookStreamWrapper_new(current_cell->stdoutStream()));
    PyDict_SetItem(sys_dict, PyBytes_FromString("stderr"),
                   SciPyNotebookStreamWrapper_new(current_cell->stderrStream()));
#else
    PyDict_SetItem(sys_dict, PyString_FromString("stdout"),
                   SciPyNotebookStreamWrapper_new(current_cell->stdoutStream()));
    PyDict_SetItem(sys_dict, PyString_FromString("stderr"),
                   SciPyNotebookStreamWrapper_new(current_cell->stderrStream()));
#endif

    // Set working directory to file dir if file path is set:
    current_cell->context()->setWorkingDirectory();

    // Get code
    QString code = current_cell->codeDocument()->toPlainText();

    // Precompile code:
    PyObject *prec_code = Py_CompileString(code.toStdString().c_str(), "<cell>", Py_file_input);
    if (0 == prec_code)
    {
      // Extract Exception:
      PyObject *exec, *pvalue, *traceback;
      PyErr_Fetch(&exec, &pvalue, &traceback);

      if (0 == pvalue) {
        qWarning("Compilation failed but no exception found.");
        current_cell->setEvaluationState(Cell::ERROR);
        continue;
      }

      // Handle syntax error exception
      if (exec == PyExc_SyntaxError) {
        // Extract the line number of the first frame and mark the line in codecell
        PyObject *lineno = PyObject_GetAttrString(pvalue, "lineno");
        if (lineno) {
          current_cell->markCodeLine(PyLong_AsSsize_t(lineno)); Py_DECREF(lineno);
        }
      } else if (0 != traceback) {
        // Extract the line number of the first frame and mark the line in codecell
        PyObject *lineno = PyObject_GetAttrString(traceback, "tb_lineno");
        if (lineno) {
          current_cell->markCodeLine(PyLong_AsSsize_t(lineno));
          Py_DECREF(lineno);
        }
      }

      // Restore exception to be printed:
      PyErr_Restore(exec, pvalue, traceback);
      PyErr_Print();

      current_cell->setEvaluationState(Cell::ERROR);
      continue;
    }

    // Evaluate code:
#if PY_MAJOR_VERSION >= 3
    PyObject *result = PyEval_EvalCode(
          prec_code, current_cell->globalContext(), current_cell->globalContext());
#else
    PyObject *result = PyEval_EvalCode(
          (PyCodeObject *)prec_code, current_cell->globalContext(), current_cell->globalContext());
#endif

    if (0 == result) {
      // Extract Exception:
      PyObject *exec, *pvalue, *traceback;
      PyErr_Fetch(&exec, &pvalue, &traceback);

      if (0 == pvalue) {
        // oops:
        qWarning("Compilation failed but no exception found.");
        current_cell->setEvaluationState(Cell::ERROR);
        continue;
      }

      // If there is a traceback:
      if (0 != traceback) {
        // Extract the line number of the first frame and mark the line in codecell
        PyObject *lineno = PyObject_GetAttrString(traceback, "tb_lineno");
        if (lineno) {
          current_cell->markCodeLine(PyLong_AsSsize_t(lineno));
          Py_DECREF(lineno);
        }
      }

      // Restore exception to be printed:
      PyErr_Restore(exec, pvalue, traceback);
      // print traceback to sys.stderr
      PyErr_Print();

      current_cell->setEvaluationState(Cell::ERROR);
      continue;
    }

    /// @todo Notify context has changed.
    current_cell->setEvaluationState(Cell::EVALUATED);

    Py_DECREF(result);
    Py_DECREF(prec_code);
  }

  PyGILState_Release(gstate);
}

