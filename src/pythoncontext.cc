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
#include "pythoncontext.hh"
#include "pythonengine.hh"

#include <QFileInfo>
#include <QDir>
#include <iostream>


PythonContext::PythonContext(QObject *parent) :
  QObject(parent), _main_module(0), _globals(0), _locals(0), _filepath("")
{
  // Instantiate python engine if not happend yet.
  PythonEngine::get();

  // Load __main__ module:
  if (0 == (_main_module = PyImport_AddModule("__main__"))) {
    qFatal("Can not import '__main__'");
    exit(-1);
  }

  // Allcocate initial variable scopes (top-level)
  if (0 == (_globals = PyDict_New())) {
    qFatal("Can not create global scope!");
    exit(-1);
  }
  if (0 == (_locals = PyDict_New())) {
    qFatal("Can not create local scope!");
    exit(-1);
  }

  // Populate local and global scope
  if (0 > PyDict_Merge(_globals, PyModule_GetDict(_main_module), 1)) {
    qCritical("Error while merging global variable scopes.");
  }
  if (0 > PyDict_Merge(_locals, _globals, 1)) {
    qCritical("Error while merging local variable scopes.");
  }
}


PythonContext::~PythonContext()
{
  // Decrement variable scopes:
  Py_DECREF(_globals);
  Py_DECREF(_locals);
  Py_DECREF(_main_module);
}


PyObject *
PythonContext::getGlobals() {
  return _globals;
}


PyObject *
PythonContext::getLocals() {
  return _locals;
}


void
PythonContext::setFileName(const QString &filename)
{
  // Store filename:
  _filepath = filename;

#if PY_MAJOR_VERSION >= 3
  // Convert string to python string
  PyObject *fname = PyBytes_FromString(_filepath.toStdString().c_str());
#else
  // Convert string to python string
  PyObject *fname = PyString_FromString(_filepath.toStdString().c_str());
#endif

  // Store filename into local and global context
  PyDict_SetItemString(_globals, "__file__", fname);
  PyDict_SetItemString(_locals, "__file__", fname);

  // Set working directory to file directory:
  QDir file_dir = QFileInfo(_filepath).dir();
  setWorkingDirectory(file_dir);
}


QString
PythonContext::fileLocation() const {
  // Skip if no filepath is set
  if (0 == _filepath.size()) { return ""; }
  // Get directory from file-path
  QDir file_dir = QFileInfo(_filepath).dir();
  return file_dir.path();
}

void
PythonContext::setWorkingDirectory()
{
  // Skip if no filepath is set
  if (0 == _filepath.size()) { return; }
  // Get directory from file-path
  QDir file_dir = QFileInfo(_filepath).dir();
  // Set working directory
  setWorkingDirectory(file_dir);
}

void
PythonContext::setWorkingDirectory(const QDir &dir)
{
  // Load sys module:
  PyObject *sys_module = 0;
  if (0 == (sys_module = PyImport_AddModule("sys"))) {
    qFatal("Can not import 'sys'");
    exit(-1);
  }

  PyObject *sys_path = 0;
  if (0 == (sys_path = PyDict_GetItemString(PyModule_GetDict(sys_module), "path"))) {
    qFatal("Can not get 'sys.path'.");
    exit(-1);
  }

#if PY_MAJOR_VERSION >= 3
  PyObject *path_str=PyBytes_FromString(dir.path().toStdString().c_str());
#else
  PyObject *path_str=PyString_FromString(dir.path().toStdString().c_str());
#endif
  if(PyList_Insert(sys_path, 0, path_str)) {
    qFatal("Can not prepend file-directory to sys.path.");
    exit(-1);
  }
}


void
PythonContext::getNamesFor(const QString &path, QStringList &names)
{
  PyObject *object = _globals; Py_INCREF(object);

  // If no path is given, assemble list from global dictionary:
  if ("" == path) {
    PyObject *key = 0; ssize_t pos = 0;
    while (PyDict_Next(_globals, &pos, &key, NULL)) {
#if PY_MAJOR_VERSION >= 3
      names.append(PyBytes_AsString(key));
#else
      names.append(PyString_AsString(key));
#endif
    }
    return;
  }

  // Find object referenced by path:
  QStringList path_list = path.split(".", QString::SkipEmptyParts);
  object = PyDict_GetItemString(_globals, path_list.first().toStdString().c_str());
  path_list.pop_front(); Py_INCREF(object);
  foreach (QString name, path_list) {
    // Get a weak reference to the item of the dictionary:
    PyObject *next = PyObject_GetAttrString(object, name.toStdString().c_str()); Py_DECREF(object);
    if (0 == next) { return; } object = next;
  }

  // Assemble list of attributes (for module object -> elements):
  PyObject *keys = 0;
  if (PyModule_Check(object)) {
    keys = PyDict_Keys(PyModule_GetDict(object)); Py_DECREF(object);
  } else {
    keys = PyObject_Dir(object); Py_DECREF(object);
  }

  // Iterate over list of elements and assemble list of names:
  for (int i=0; i<PyList_Size(keys); i++) {
#if PY_MAJOR_VERSION >= 3
    names.append(PyBytes_AsString(PyList_GetItem(keys, i)));
#else
    names.append(PyString_AsString(PyList_GetItem(keys, i)));
#endif
  }
}

