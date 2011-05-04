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

#ifndef PYTHONCONTEXT_HH
#define PYTHONCONTEXT_HH

#include <QObject>
#include <Python.h>
#include <QStringListModel>


class PythonContext : public QObject
{
    Q_OBJECT

protected:
    PyObject *_globals;
    PyObject *_locals;

    QString _current_names_prefix;
    QStringListModel *_names;

public:
    explicit PythonContext(QObject *parent = 0);

    PyObject *getGlobals();
    PyObject *getLocals();

    void setFileName(const QString &filename);

    void updateGlobalNames();
    QStringListModel *getNamesOf(const QString &prefix);

public:
  void updateNamesFrom(QStringList &prefix);
  void updateNamesFrom(PyObject *object, QStringList &prefix);
};

#endif // PYTHONCONTEXT_HH
