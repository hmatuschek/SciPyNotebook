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

#ifndef __SCIPY_NOTEBOOK_CELL_H__
#define __SCIPY_NOTEBOOK_CELL_H__

#include <QObject>
#include <QVBoxLayout>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QTimer>
#include "pythoncompleter.hh"
#include "cellinputstream.hh"

// Foward declarations
class Notebook;

/**
 * The cell class combines two cells, one for the input of code and another for the
 * output, this code generates or error messages.
 */
class Cell : public QObject
{
  Q_OBJECT

public:
  typedef enum {
    UNEVALUATED,
    QUEUED,
    EVALUATING,
    EVALUATED,
    ERROR
  } EvaluationState;

public:
  /** Construct a new (empty) cell.
   * \param notebook Specifies a weak reference to the notebook, that owns this cell.
   */
  explicit Cell(Notebook *notebook);

  /** Serializes the code of the codecell into the given device. */
  void serializeCode(QIODevice &device);

  /** Replaces the code in the codecell with the given text. */
  void setCode(const QString &code);

  QTextDocument *codeDocument();
  QTextDocument *resultDocument();

  CellInputStream *stdoutStream();
  CellInputStream *stderrStream();

  void setEvaluationState(EvaluationState state);
  EvaluationState evaluationState();

  PyObject *globalContext();
  PyObject *localContext();

  PythonCompleter *completer();

  void markCodeLine(size_t line);
  void setSplitPosition(size_t pos);
  size_t splitPosition() const;

  bool isModified() const;
  void setModified(bool modified=true);

public slots:
  void setActive();
  void setInactive();

signals:
  void cellActivated(Cell *active_cell);
  void cellDeactivated(Cell *active_cell);
  void evaluationStateChanged(unsigned int old_state, unsigned int new_state);
  void modifiedStateChanged(bool modified);
  void highlightLine(int line);

private slots:
  void onStderrText(QString text);
  void onStdoutText(QString text);
  void onCodeChanged();


protected:
  Notebook *_notebook;
  EvaluationState _evaluation_state;
  bool _is_modified;

  /** Holds the instance of the code cell. */
  QTextDocument *_codedocument;

  /** Holds the instance of the result cell. */
  QTextDocument *_resultdocument;

  /** Hold the stdout stream to the result cell. */
  CellInputStream _stdoutStream;
  /** Hold the stderr stream to the result cell. */
  CellInputStream _stderrStream;

  QTextCharFormat _stdoutFormat;
  QTextCharFormat _stderrFormat;

  PythonCompleter *_completer;
  size_t _split_position;
};

#endif // __SCIPY_NOTEBOOK_CELL_H__
