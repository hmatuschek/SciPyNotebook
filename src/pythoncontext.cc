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

#include <QFileInfo>
#include <QDir>
#include <iostream>


PythonContext::PythonContext(QObject *parent) :
    QObject(parent)
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
  // Convert string to python string
  PyObject *fname = PyString_FromString(filename.toStdString().c_str());

  // Store filename into local and global context
  PyDict_SetItemString(_globals, "__file__", fname);
  PyDict_SetItemString(_locals, "__file__", fname);

  // Append directory of file to sys.path
  QDir file_dir = QFileInfo(filename).dir();

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

  if(PyList_Insert(sys_path, 0, PyString_FromString(file_dir.path().toStdString().c_str()))) {
    qFatal("Can not prepend file-directory to sys.path.");
    exit(-1);
  }
}


void
PythonContext::getNamesFor(const QString &path, QStringList &names)
{
  PyObject *object = _globals; Py_INCREF(object);

  if ("" != path) {
    foreach (QString name, path.split(".", QString::SkipEmptyParts)) {
      PyObject *next = PyDict_GetItemString(object, name.toStdString().c_str());
      Py_DECREF(object);
      if (0 == next) { return; }
      object = PyObject_Dir(next);
    }
  }

  PyObject *key = 0; ssize_t pos = 0;
  while (PyDict_Next(object, &pos, &key, NULL)) {
    names.append(PyString_AsString(key));
  }
}

