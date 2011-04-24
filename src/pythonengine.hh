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

#ifndef PYTHONENGINE_HH
#define PYTHONENGINE_HH

#include <QObject>

#include <Python.h>
#include "cellinputstream.hh"


class PythonEngine: public QObject
{
    Q_OBJECT;

protected:
    static PythonEngine *instance;
    PyObject *_locals;
    PyObject *_globals;

protected:
    explicit PythonEngine(QObject *parent=0);

public:
    static PythonEngine *get();

    ~PythonEngine();

    PyObject *getGlobals();
    PyObject *getLocals();

    void setStdout(CellInputStream *stream);
    void setStderr(CellInputStream *stream);
};



#endif // PYTHONENGINE_HH
