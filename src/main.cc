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
#include "preferences.hh"
#include "application.hh"

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    NotebookWindow *window = 0;
    if(2 == argc) {
      // openfile give by cmd line
      window = new NotebookWindow(argv[1]);
    } else {
      // Open new, empty notebook window
      window = new NotebookWindow();
    }

    window->show();

    // Execute application
    int ret_val = a.exec();

    // Free window
    delete window;

    // Shutdown python engine:
    //PythonEngine::shutdown();

    // Done.
    return ret_val;
}
