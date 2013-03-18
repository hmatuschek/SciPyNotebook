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
    UNEVALUATED,  ///< The cell is unevaluated/modified.
    QUEUED,       ///< The cell is queued for evaluation.
    EVALUATING,   ///< The cell is being evaluated.
    EVALUATED,    ///< The cell was successfully evaluated.
    ERROR         ///< The evaluation of the cell returned an error.
  } EvaluationState;

public:
  /** Construct a new (empty) cell.
   * \param notebook Specifies a weak reference to the notebook, that owns this cell. */
  explicit Cell(Notebook *notebook);

  /** Serializes the code of the codecell into the given device. */
  void serializeCode(QIODevice &device);

  /** Replaces the code in the codecell with the given text. */
  void setCode(const QString &code);
  /** Returns the code @c QTextDocument. */
  QTextDocument *codeDocument();
  /** Returns the result (stderr & stdout) @c QTextDocument. */
  QTextDocument *resultDocument();
  /** Returns an input stream representing stdout. */
  CellInputStream *stdoutStream();
  /** Returns an input stream representing stderr. */
  CellInputStream *stderrStream();

  /** Resets the evaluation state, also emits @c evaluationStateChanged signal. */
  void setEvaluationState(EvaluationState state);
  /** Returns the current evaluation state. */
  EvaluationState evaluationState();

  /** Returns the global evaluation context as a @c PyDict.*/
  PyObject *globalContext();
  /** Returns the local evaluation context as a @c PyDict.*/
  PyObject *localContext();
  /** Returns the evaluation context.*/
  PythonContext *context();

  /** Returns the parent notebook. */
  Notebook *notebook();

  /** Emits a @c markCodeLine signal. */
  void markCodeLine(size_t line);

  /** Retunrs true if the cell is modified. */
  bool isModified() const;
  /** Resets the "isModified" state, emits @c modifiedStateChanged signal. */
  void setModified(bool modified=true);

signals:
  void evaluationStateChanged(unsigned int old_state, unsigned int new_state);
  void modifiedStateChanged(bool modified);
  void highlightLine(int line);

private slots:
  void onStderrText(QString text);
  void onStdoutText(QString text);
  void onCodeChanged();


protected:
  /** Holds a weak reference to the notebook, this cell belongs to. */
  Notebook *_notebook;
  /** Holds the evaluations state of the cell. */
  EvaluationState _evaluation_state;
  /** If true, this cell was modified. */
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
};

#endif // __SCIPY_NOTEBOOK_CELL_H__
