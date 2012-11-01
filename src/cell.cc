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

#include "cell.hh"
#include "cellstatus.hh"
#include "pythonengine.hh"
#include "pythoncontext.hh"
#include "pythoncompleter.hh"
#include "notebook.hh"

#include <QStringListModel>
#include <QTextCursor>

#include <iostream>


Cell::Cell(Notebook *notebook) :
  QObject(notebook), _notebook(notebook), _stdoutStream(), _stderrStream(), _split_position(0),
  _is_modified(true)
{
  // Allocate documents for code && result:
  _codedocument = new QTextDocument(this);
  _resultdocument = new QTextDocument(this);

  // Setup format for stderr and stdout...
  _stdoutFormat.setFontStyleHint(QFont::TypeWriter, QFont::PreferMatch);
  _stdoutFormat.setForeground(QColor(0x00, 0x00, 0x00));
  _stderrFormat.setFontStyleHint(QFont::TypeWriter, QFont::PreferMatch);
  _stderrFormat.setForeground(QColor(0xa0, 0x00, 0x00));

  // Setup streams:
  QObject::connect(&_stdoutStream, SIGNAL(newData(QString)), this, SLOT(onStdoutText(QString)));
  QObject::connect(&_stderrStream, SIGNAL(newData(QString)), this, SLOT(onStderrText(QString)));
  QObject::connect(_codedocument, SIGNAL(contentsChanged()), this, SLOT(onCodeChanged()));
}


void
Cell::setEvaluationState(EvaluationState state) {
  emit evaluationStateChanged(_evaluation_state, state);
  _evaluation_state = state;
}

Cell::EvaluationState
Cell::evaluationState() {
  return _evaluation_state;
}

void
Cell::serializeCode(QIODevice &device) {
  // Remove tailing newlines:
  device.write(_codedocument->toPlainText().toAscii().trimmed());
}

void
Cell::setCode(const QString &code) {
  _codedocument->setPlainText(code);
  _codedocument->adjustSize();
}

QTextDocument *
Cell::codeDocument() {
  return _codedocument;
}

QTextDocument *
Cell::resultDocument() {
  return _resultdocument;
}


CellInputStream *
Cell::stdoutStream() {
  return &_stdoutStream;
}

CellInputStream *
Cell::stderrStream() {
  return &_stderrStream;
}

PyObject *
Cell::globalContext() {
  return _notebook->context()->getGlobals();
}

PyObject *
Cell::localContext() {
  return _notebook->context()->getLocals();
}

PythonContext *
Cell::context() {
  return _notebook->context();
}

Notebook *
Cell::notebook() {
  return _notebook;
}


size_t
Cell::splitPosition() const {
  return _split_position;
}

bool
Cell::isModified() const {
  return _is_modified;
}

void
Cell::setModified(bool modified) {
  _is_modified = modified;
  emit modifiedStateChanged(_is_modified);
}


void
Cell::setSplitPosition(size_t pos) {
  QTextCursor cursor(_codedocument);
  cursor.setPosition(pos); cursor.movePosition(QTextCursor::StartOfLine);
  _split_position = cursor.position();
}

void
Cell::markCodeLine(size_t line) {
  emit highlightLine(line);
}

void
Cell::setActive() {
  emit cellActivated(this);
}

void
Cell::setInactive() {
  emit cellDeactivated(this);
}


void
Cell::onStdoutText(QString text) {
  QTextCursor cursor(_resultdocument);
  cursor.movePosition(QTextCursor::End);
  cursor.insertText(text, _stdoutFormat);
}

void
Cell::onStderrText(QString text) {
  QTextCursor cursor(_resultdocument);
  cursor.movePosition(QTextCursor::End);
  cursor.insertText(text, _stderrFormat);
}

void
Cell::onCodeChanged() {
  setEvaluationState(Cell::UNEVALUATED);
  setModified(true);
}
