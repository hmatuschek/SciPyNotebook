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

#include "pythonengine.hh"
#include "streamwrapper.hh"

#include <iostream>


PythonEngine *PythonEngine::instance = 0;





PythonEngine::PythonEngine(QObject *parent) :
        QObject(parent)
{
    Py_Initialize();
    PythonEngine::instance = this;

    // Register SciPyNotebookStreamWrapper class:
    if (0 > SciPyNotebookStreamWrapperType_init())
    {
        std::cerr << "Oops: Can not register stream wrapper type..." << std::endl;
        exit(0);
    }

    std::cerr << "Python interpreter started..." << std::endl;
}



PythonEngine::~PythonEngine()
{
    std::cerr << "Finalize python interpreter..." << std::endl;
    Py_Finalize();
    PythonEngine::instance = 0;
}


PythonEngine *
PythonEngine::get()
{
    if (0 != PythonEngine::instance)
    {
        return PythonEngine::instance;
    }

    PythonEngine::instance = new PythonEngine();
    return PythonEngine::instance;
}


void
PythonEngine::setStdout(CellInputStream *stream)
{
    // Load sys:
    PyObject *sys_module = 0;
    if (0 == (sys_module = PyImport_AddModule("sys")))
    {
        std::cerr << "Opps: Can not import 'sys'" << std::endl;
        exit(-1);
    }

    PyObject *sys_dict = PyModule_GetDict(sys_module);
    PyDict_SetItem(sys_dict, PyString_FromString("stdout"),
                   SciPyNotebookStreamWrapper_new(stream));
}


void
PythonEngine::setStderr(CellInputStream *stream)
{
    // Load sys:
    PyObject *sys_module = 0;
    if (0 == (sys_module = PyImport_AddModule("sys")))
    {
        std::cerr << "Opps: Can not import 'sys'" << std::endl;
        exit(-1);
    }

    PyObject *sys_dict = PyModule_GetDict(sys_module);
    PyDict_SetItem(sys_dict, PyString_FromString("stderr"),
                   SciPyNotebookStreamWrapper_new(stream));
}


bool
PythonEngine::isRunning()
{
    return 0 != PythonEngine::instance;
}

