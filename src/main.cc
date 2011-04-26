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

#include <QtGui/QApplication>
#include "notebookwindow.hh"
#include "pythonengine.hh"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create a new, empty notbook
    NotebookWindow w;
    w.show();

    int ret_val = a.exec();

    // Shutdown python engine:
    if (PythonEngine::isRunning())
        delete PythonEngine::get();

    return ret_val;
}
