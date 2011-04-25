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

#ifndef __SCIPY_NOTEBOOK_STREAMWRAPPER_HH__
#define __SCIPY_NOTEBOOK_STREAMWRAPPER_HH__

#include <Python.h>
#include "cellinputstream.hh"


/**
 * Registers the StreamWrapperType with the python type system.
 */
int SciPyNotebookStreamWrapperType_init();


/**
 * Constructs a StreamWrapper instance forwarding to CellInputStream instance.
 */
PyObject *SciPyNotebookStreamWrapper_new(CellInputStream *stream);


/**
 * Defines the StreamWrapper python class.
 */
typedef struct {
    PyObject_HEAD

    /**
     * Holds the stream instance the data is forwarded to.
     */
    CellInputStream *stream;
} SciPyNotebookStreamWrapper;


#endif // __SCIPY_NOTEBOOK_STREAMWRAPPER_HH__
