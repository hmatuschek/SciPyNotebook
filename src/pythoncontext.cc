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

#include <iostream>


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

    // Instantiate listmodel of all symbols defined
    this->_names = new QStringListModel();

    // update names from global context
    this->updateGlobalNames();
}


PythonContext::~PythonContext()
{
  // Free ListModel for names.
  delete this->_names;
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


void
PythonContext::setFileName(const QString &filename)
{
    // Convert string to python string
    PyObject *fname = PyString_FromString(filename.toStdString().c_str());

    // Store filename into local and global context
    PyDict_SetItemString(this->_globals, "__file__", fname);
    PyDict_SetItemString(this->_locals, "__file__", fname);
}


QStringListModel *
PythonContext::getNamesOf(const QString &prefix)
{
  if (this->_current_names_prefix != prefix)
  {
    this->_current_names_prefix = prefix;

    if ("" == prefix)
    {
      this->updateGlobalNames();
    }
    else
    {
      QStringList prefix_list = prefix.split('.', QString::SkipEmptyParts);
      this->updateNamesFrom(prefix_list);
    }
  }

  return this->_names;
}


void
PythonContext::updateGlobalNames()
{
  QStringList name_list;

  PyObject *key = 0; ssize_t pos = 0;
  while (PyDict_Next(this->_globals, &pos, &key, NULL))
  {
    name_list.append(PyString_AsString(key));
  }

  this->_names->setStringList(name_list);
}


void
PythonContext::updateNamesFrom(QStringList &prefix)
{
  if (0 == prefix.length())
  {
    this->updateGlobalNames();
    return;
  }

  PyObject *object = 0;
  if (0 == (object = PyDict_GetItemString(this->_globals, prefix.front().toStdString().c_str())))
  {
    QStringList name_list;
    this->_names->setStringList(name_list);
    return;
  }

  // Remove first name from prefix list:
  prefix.pop_front();
  this->updateNamesFrom(object, prefix);
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


