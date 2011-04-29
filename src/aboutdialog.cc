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

#include "aboutdialog.hh"
#include <QTextEdit>

static QString content = \
"<html>" \
"<head></head>" \
"<body>" \
"<h1 align=\"center\">SciPy Notebook</h1>" \
"<center><b>Version 0.2.0</b></center>" \
"<center>(c) 2011 <i>Hannes Matuschek</i></center>" \
"<p> This program is free software; you can redistribute it and/or modify" \
"it under the terms of the GNU General Public License as published by" \
"the Free Software Foundation; either version 2 of the License, or" \
"(at your option) any later version.</p>"\
"<p>This program is distributed in the hope that it will be useful," \
"but WITHOUT ANY WARRANTY; without even the implied warranty of" \
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" \
"GNU General Public License for more details.</p>" \
"<p>You should have received a copy of the GNU General Public License along" \
"with this program; if not, write to the Free Software Foundation, Inc.," \
"51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.</p>" \
"</body>" \
"</html>";


AboutDialog::AboutDialog(QWidget *parent) :
    QMainWindow(parent)
{
  QTextEdit *view = new QTextEdit(this);
  view->setReadOnly(true);
  view->setHtml(content);

  this->setCentralWidget(view);
  this->resize(320, 240);
}
