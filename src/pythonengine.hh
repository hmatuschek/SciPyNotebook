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

#ifndef PYTHONENGINE_HH
#define PYTHONENGINE_HH

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <Python.h>
#include "cellinputstream.hh"


// Forward declaration
class Cell;


/** This class implements the Python execution engine, which is run in a separate thread to
 * avoid a blocking GUI when evaluating python code. */
class PythonEngine: protected QThread
{
  Q_OBJECT;

protected:
  /** Hidden constructor, avoids direct instantiation. */
  explicit PythonEngine(QObject *parent=0);

  /** Implements the execution part. */
  virtual void run();

public:
  /** Factory method. */
  static PythonEngine *get();
  /** Destructor. */
  virtual ~PythonEngine();

  /** Adds a cell to the queue. */
  void queueCell(Cell *cell);

protected:
  /** Holds the singleton instance of the engine. */
  static PythonEngine *_instance;
  /** Is true while the thread is running. */
  bool _isrunning;

  /** The cell queue. */
  QList<Cell *> _queue;
  /** The queue mutex. */
  QMutex _mutex;
  /** The consumer condition. */
  QWaitCondition _notEmptyCondition;
};



#endif // PYTHONENGINE_HH
