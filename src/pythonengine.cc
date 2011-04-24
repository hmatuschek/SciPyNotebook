/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

    // Register SciPyStudioStreamWrapper class:
    if (0 > SciPyStudioStreamWrapperType_init())
    {
        std::cerr << "Oops: Can not register stream wrapper..." << std::endl;
        exit(0);
    }

    // Allcocate initial variable scopes (top-level)
    this->_locals = PyDict_New();
    this->_globals = PyDict_New();

    if (0 == this->_globals || 0 == this->_locals)
    {
        std::cerr << "Oops: Can not create scopes..." << std::endl;
        exit(0);
    }

    // Load __main__ module:
    PyObject *main_module = 0;
    if (0 == (main_module = PyImport_AddModule("__main__")))
    {
        std::cerr << "Opps: Can not import '__main__'" << std::endl;
        exit(-1);
    }

    // Populate local and global scope
    PyDict_Merge(this->_globals, PyModule_GetDict(main_module), 0);
    PyDict_Merge(this->_locals, this->_globals, 0);

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


PyObject *
PythonEngine::getGlobals()
{
    return this->_globals;
}


PyObject *
PythonEngine::getLocals()
{
    return this->_locals;
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
                   SciPyStudioStreamWrapper_new(stream));
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
                   SciPyStudioStreamWrapper_new(stream));
}

