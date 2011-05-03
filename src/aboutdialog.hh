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

#ifndef ABOUTDIALOG_HH
#define ABOUTDIALOG_HH

#include <QDialog>

/**
 * Shows some information about SciPy Notebook.
 *
 * \todo Buityfy
 */
class AboutDialog : public QDialog
{
    Q_OBJECT


public:
  /**
   * Contructs a new dialog.
   */
  explicit AboutDialog(QWidget *parent = 0);

};

#endif // ABOUTDIALOG_HH
