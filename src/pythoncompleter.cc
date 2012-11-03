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

#include "pythoncompleter.hh"
#include <iostream>


PythonCompleter::PythonCompleter(PythonContext *ctx, QObject *parent) :
    QCompleter(parent), _context(ctx)
{
  _model = new QStringListModel();
  setModel(_model);
}


void
PythonCompleter::setCompletionPrefix(const QString &prefix)
{
  // Split prefix in object path and name prefix:
  QStringList names = prefix.split(".", QString::KeepEmptyParts);
  QString name_prefix = names.back(); names.pop_back();
  QString object_path = names.join(".");

  QStringList lst; _context->getNamesFor(object_path, lst);
  _model->setStringList(lst);
  QCompleter::setCompletionPrefix(name_prefix);
}
