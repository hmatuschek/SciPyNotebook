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
#include "streamwrapper.hh"
#include "cellinputstream.hh"
#include <iostream>

/** Defines the StreamWrapper python class. */
typedef struct {
    PyObject_HEAD

    /**
     * Holds the stream instance the data is forwarded to.
     */
    CellInputStream *stream;
} SciPyNotebookStreamWrapper;



/**
 * Implements the "write()" method for the StreamWrapper objects.
 */
static PyObject *
SciPyNotebookStreamWrapper_write(SciPyNotebookStreamWrapper *self, PyObject *args)
{
  // Get text to write from args
  PyObject *text = 0;
  if(!PyArg_ParseTuple(args, "S", &text)) {
    std::cerr << "Invalid args..." << std::endl;
    return NULL;
  }

  // Extract string and send it to stream:
#if PY_MAJOR_VERSION >= 3
  self->stream->write(PyBytes_AsString(text));
#else
  self->stream->write(PyString_AsString(text));
#endif
  // Return number of bytes send
  Py_RETURN_NONE;
}


/** The method definitions for StreamWrapper instances. */
static PyMethodDef SciPyNotebookStreamWrapperType_methods[] =
{
  {"write", (PyCFunction)SciPyNotebookStreamWrapper_write, METH_VARARGS, "..."},
  {NULL, NULL, 0, NULL}
};


/**
 * Defines the StreamWrapper type.
 */
static PyTypeObject SciPyNotebookStreamWrapperType = {
  #if PY_MAJOR_VERSION >= 3
  PyVarObject_HEAD_INIT(NULL, 0)
  "SciPyNotebookStreamWrapper",
  0, sizeof(SciPyNotebookStreamWrapperType),
  0,0,0,0,0,0, 0,0,0, 0,0,0,0,0, 0,
  Py_TPFLAGS_DEFAULT,
  "Wrapper of stderr and stdout.",
  0,0,0,0,0,0,
  SciPyNotebookStreamWrapperType_methods
  #else
  PyObject_HEAD_INIT(NULL)
  0,
  "SciPyNotebookStreamWrapper",
  sizeof(SciPyNotebookStreamWrapperType),
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  Py_TPFLAGS_DEFAULT,
  "Wrapper of stderr and stdout.",
  0,0,0,0,0,0,
  SciPyNotebookStreamWrapperType_methods
  #endif
};



PyObject *
SciPyNotebookStreamWrapper_new(CellInputStream *stream)
{
  SciPyNotebookStreamWrapper *self;

  if (0 == (self = (SciPyNotebookStreamWrapper *)SciPyNotebookStreamWrapperType.tp_alloc(&SciPyNotebookStreamWrapperType, 0))) {
    return 0;
  }

  // Init object
  self = (SciPyNotebookStreamWrapper *)PyObject_Init((PyObject *)self, &SciPyNotebookStreamWrapperType);

  // Store stream instance:
  self->stream = stream;

  return (PyObject *) self;
}


int
SciPyNotebookStreamWrapperType_init()
{
    if (0 > PyType_Ready(&SciPyNotebookStreamWrapperType))
    {
        return -1;
    }

    Py_INCREF(&SciPyNotebookStreamWrapperType);

    return 0;
}
