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


class PythonContext : public QObject
{
    Q_OBJECT

protected:
    PyObject *_globals;
    PyObject *_locals;

public:
    explicit PythonContext(QObject *parent = 0);

    PyObject *getGlobals();
    PyObject *getLocals();
};

#endif // PYTHONCONTEXT_HH
