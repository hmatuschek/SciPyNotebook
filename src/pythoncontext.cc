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

    // Instantiate listmodel of all symbols defined
    _names = new QStringListModel();

    // update names from global context
    updateGlobalNames();
}


PythonContext::~PythonContext()
{
  // Free ListModel for names.
  delete _names;

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


QStringListModel *
PythonContext::getNamesOf(const QString &prefix)
{
  if (_current_names_prefix != prefix)
  {
    _current_names_prefix = prefix;

    if ("" == prefix) {
      updateGlobalNames();
    } else {
      QStringList prefix_list = prefix.split('.', QString::SkipEmptyParts);
      updateNamesFrom(prefix_list);
    }
  }

  return _names;
}


void
PythonContext::updateGlobalNames()
{
  QStringList name_list;

  PyObject *key = 0; ssize_t pos = 0;
  while (PyDict_Next(_globals, &pos, &key, NULL)) {
    name_list.append(PyString_AsString(key));
  }

  _names->setStringList(name_list);
}


void
PythonContext::updateNamesFrom(QStringList &prefix)
{
  if (0 == prefix.length()) {
    updateGlobalNames();
    return;
  }

  PyObject *object = 0;
  if (0 == (object = PyDict_GetItemString(_globals, prefix.front().toStdString().c_str()))) {
    QStringList name_list;
    _names->setStringList(name_list);
    return;
  }

  // Remove first name from prefix list:
  prefix.pop_front();
  updateNamesFrom(object, prefix);
}


void
PythonContext::updateNamesFrom(PyObject *object, QStringList &prefix)
{
  QStringList name_list;

  if (0 == prefix.length())
  {
    PyObject *items = 0;
    if (0 == (items = PyObject_Dir(object)))
    {
      PyErr_Clear();

      // Set empty list:
      this->_names->setStringList(name_list);
      // done.
      return;
    }

    // get iterator for list:
    PyObject *iterator = 0;

    if (0 == (iterator = PyObject_GetIter(items)))
    {
      // oops
      this->_names->setStringList(name_list);
      PyErr_Clear();
      Py_DECREF(items);
      // done.
      return;
    }

    // Iterate over list:
    PyObject *item = 0;
    while(0 != (item = PyIter_Next(iterator)))
    {
      name_list.append(PyString_AsString(item));
      Py_DECREF(item);
    }

    if (PyErr_Occurred()) {
      // oops, clear name list and python-error and return!
      name_list.clear();
      PyErr_Clear();
    }

    Py_DECREF(iterator);
    Py_DECREF(items);

    // set list of names:
    this->_names->setStringList(name_list);

    return;
  }

  PyObject *child = 0;
  if (0 == (child = PyObject_GetAttrString(object, prefix.front().toStdString().c_str())))
  {
    // Set empty name list:
    this->_names->setStringList(name_list);
    return;
  }

  // pop name from prefix:
  prefix.pop_front();

  // continue:
  this->updateNamesFrom(child, prefix);

  // free child:
  Py_DECREF(child);
}


