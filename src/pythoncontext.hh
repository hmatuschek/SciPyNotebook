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

#ifndef PYTHONCONTEXT_HH
#define PYTHONCONTEXT_HH

#include <QObject>
#include <Python.h>
#include <QStringListModel>
#include <QDir>


/**
 * This class holds the python execution context for a notebook. This context contains the
 * global and local scope of the notebook as well as the file-name and working directory.
 */
class PythonContext : public QObject
{
  Q_OBJECT

protected:
  /** Holds the built-in __main__ module for this context. */
  PyObject *_main_module;
  /** Holds the global scope for this context. */
  PyObject *_globals;
  /** Holds the local scope for this context. */
  PyObject *_locals;
  /** Holds the file-path for the this context. */
  QString _filepath;

public:
  /** Constucts a new python context.  */
  explicit PythonContext(QObject *parent = 0);
  /** Destructor. */
  virtual ~PythonContext();
  /** Returns the global scope as a @c PyDict. */
  PyObject *getGlobals();
  /** Returns the local scope as a @c PyDict. */
  PyObject *getLocals();

  /** Sets the file-name of the context. */
  void setFileName(const QString &filename);
  /** Sets the working directory of the context to the directory of the file-path. */
  void setWorkingDirectory();
  /** Sets the working directory of the context to the given dir. */
  void setWorkingDirectory(const QDir &dir);
  /** Collects all attributes of the given path. If path is empty, returns the elements of the
   * global scope. */
  void getNamesFor(const QString &path, QStringList &names);
};

#endif // PYTHONCONTEXT_HH
