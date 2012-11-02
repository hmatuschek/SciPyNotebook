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

#ifndef PYTHONCOMPLETER_HH
#define PYTHONCOMPLETER_HH

#include <QCompleter>
#include "pythoncontext.hh"


/**
 * This class implements a @c QCompleter resolving accessable items for a given @c PythonContext.
 */
class PythonCompleter : public QCompleter
{
  Q_OBJECT

public:
  /** Constructs a completer for the given context. */
  explicit PythonCompleter(PythonContext *ctx, QObject *parent = 0);

public slots:
  /** Updates the completion list for the given prefix. If the prefix starts with some
   * qualified names for an object, the attributes of that object are used for completion. */
  virtual void setCompletionPrefix(const QString &prefix);

private:
  /** Holds the execution context for the completer. */
  PythonContext *_context;
  /** Holds the list model for the completer. */
  QStringListModel *_model;
};

#endif // PYTHONCOMPLETER_HH
