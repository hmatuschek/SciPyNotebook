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

#include "pythoncontext.hh"
#include "pythonengine.hh"


PythonContext::PythonContext(QObject *parent) :
    QObject(parent)
{
    // Instantiate python engine if not happend yet.
    PythonEngine::get();

    // Allcocate initial variable scopes (top-level)
    this->_locals = PyDict_New();
    this->_globals = PyDict_New();

    if (0 == this->_globals || 0 == this->_locals)
    {
        qFatal("Can not create scopes...");
        exit(-1);
    }

    // Load __main__ module:
    PyObject *main_module = 0;
    if (0 == (main_module = PyImport_AddModule("__main__")))
    {
        qFatal("Can not import '__main__'");
        exit(-1);
    }

    // Populate local and global scope
    if (0 > PyDict_Merge(this->_globals, PyModule_GetDict(main_module), 1))
    {
        qCritical("Error while merging global variable scopes.");
    }

    if (0 > PyDict_Merge(this->_locals, this->_globals, 0))
    {
        qCritical("Error while merging local variable scopes.");
    }
}


PyObject *
PythonContext::getGlobals()
{
    return this->_globals;
}


PyObject *
PythonContext::getLocals()
{
    return this->_locals;
}