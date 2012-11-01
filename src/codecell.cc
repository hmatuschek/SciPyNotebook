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

#include "codecell.hh"
#include "preferences.hh"

#include <QSyntaxHighlighter>
#include <QFontMetrics>
#include <QAbstractItemView>
#include <QScrollBar>
#include <iostream>



CodeCell::CodeCell(Cell *cell, QWidget *parent) :
  QTextEdit(parent), _cell(cell)
{
  // Set the code document:
  setDocument(cell->codeDocument());

  // Create and install code completer:
  _completer = new PythonCompleter(cell->context(), this);
  _completer->setWidget(this);
  _completer->setCompletionMode(QCompleter::PopupCompletion);
  _completer->setCaseSensitivity(Qt::CaseSensitive);
  QObject::connect(_completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));

  // Configure text edit:
  setWordWrapMode(QTextOption::NoWrap);
  setAutoFormatting(QTextEdit::AutoNone);
  setAcceptRichText(false);
  setAcceptDrops(true);

  // create and install syntax highlighter
  _higlighter = new PythonHighlighter(this);
  _higlighter->setDocument(document());

  // Get default font from preferences:
  Preferences *prefs = Preferences::get();
  document()->setDefaultFont(prefs->font());

  // Set tab size:
  int tabsize = QFontMetrics(prefs->font()).width(' ');
  tabsize *= prefs->tabSize();
  setTabStopWidth(tabsize);

  // Layout specific stuff...
  _text_size = document()->size().toSize();
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  setFrameShape(QFrame::NoFrame);

  QObject::connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
  QObject::connect(_cell, SIGNAL(destroyed()), this, SLOT(onCellDeleted()));
  QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorMoved()));
}


CodeCell::~CodeCell() {
  // destroy the syntax highlighter
  delete _higlighter;
}


QSize
CodeCell::minimumSizeHint() const {
  return document()->size().toSize();
}


QSize
CodeCell::sizeHint() const {
  QSize csize(QTextEdit::sizeHint());
  csize.setHeight(document()->size().toSize().height());
  return csize;
}


void
CodeCell::onTextChanged() {
  // If there are any line-marks -> clear them
  if (0 != extraSelections().size())
    clearLineMarks();

  // Updated text height and width
  _text_size = this->document()->size().toSize();
  updateGeometry();
}


void
CodeCell::markLine(size_t line) {
  QTextEdit::ExtraSelection highlight;

  // Move cursor to position
  highlight.cursor = this->textCursor();
  highlight.cursor.movePosition(QTextCursor::Start);
  highlight.cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line-1);

  // Highlight line
  highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
  highlight.format.setBackground( Qt::yellow );

  QList<QTextEdit::ExtraSelection> extras;
  extras << highlight;
  setExtraSelections( extras );
}


void
CodeCell::clearLineMarks() {
  // just reset with empty extra selections
  QList<QTextEdit::ExtraSelection> extras;
  this->setExtraSelections(extras);
}


void
CodeCell::insertCompletion(const QString &completion) {
  if (_completer->widget() != this)
    return;

  QTextCursor tc = textCursor();
  int extra = completion.length() - _completer->completionPrefix().length();
  tc.movePosition(QTextCursor::Left);
  tc.movePosition(QTextCursor::EndOfWord);
  tc.insertText(completion.right(extra));
  setTextCursor(tc);
}


void
CodeCell::onCellDeleted() {
  _cell = 0;
  setDocument(0);
}


QString
CodeCell::textUnderCursor()
{
  // Select current "word" this includes dotted names like "sys.version" etc
  QTextCursor tc = textCursor();
  int current_position = tc.position();

  // First, move to start of dotted word:
  moveToStartOfWord(tc);

  // If cursor is at start of the word -> do nothing (for indentation)
  if(current_position == tc.position()) { return ""; }

  // start selection
  moveToEndOfWord(tc);
  return tc.selectedText();
}


void
CodeCell::moveToStartOfWord(QTextCursor &cursor)
{
  // Simply move the cursor to start of the word.
  cursor.movePosition(QTextCursor::StartOfWord);

  // If cursor is at start of text -> done;
  if (cursor.atStart()) {
    return;
  }

  // Move one char left to get previous char:
  cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);

  // Check if char is ".", if not -> done.
  if ('.' != document()->characterAt(cursor.position())) {
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    return;
  }

  // Recursively move to previous word:
  cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
  moveToStartOfWord(cursor);
}


void
CodeCell::moveToEndOfWord(QTextCursor &cursor)
{
  // Simply move the cursor to start of the word.
  cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

  // If cursor is at end of text -> done;
  if (cursor.atEnd()) { return; }

  // Check if current char is ".", if not -> done.
  if ('.' != document()->characterAt(cursor.position())) {
    return;
  }

  // Move cursor on char right:
  cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);

  // Recursively move to next word:
  moveToEndOfWord(cursor);
}


void
CodeCell::keyPressEvent(QKeyEvent *e)
{
  // If the completer is visible:
  if (_completer && _completer->popup()->isVisible()) {
    // The following keys are forwarded by the completer to the widget
    switch (e->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Escape:
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
      e->ignore();
      return; // let the completer do default behavior

    default:
      break;
    }
  }

  // Handle auto-indent:
  if ((e->modifiers() == Qt::NoModifier) && (e->key() == Qt::Key_Return)) {
    // Get whitespaces of the current line:
    QTextCursor cursor = textCursor(); cursor.select(QTextCursor::LineUnderCursor);
    QString whitespace = cursor.selectedText(); QRegExp wsexpr("^(\\s+)");
    if (0 <= wsexpr.indexIn(whitespace)) { whitespace = wsexpr.cap(1); }
    else { whitespace = ""; }
    // Forward event to QTextEdit
    QTextEdit::keyPressEvent(e);
    // If auto indent is enabled -> append the same amount of whitespaces to the new line
    if (Preferences::get()->autoIndent()) {
      textCursor().insertText(whitespace);
    }
    // done.
    return;
  }

  // Handle arrow down key press, if in last line, move focus to next cell. */
  if ((e->modifiers() == Qt::NoModifier) && (e->key() == Qt::Key_Down)) {
    QTextCursor cursor = textCursor(); cursor.movePosition(QTextCursor::EndOfLine);
    QWidget *next = nextInFocusChain();
    if (cursor.atEnd()) { std::cerr << "move focus to next widget: " << next << std::endl; }
    if (cursor.atEnd() && next) { next->setFocus(); }
    else { QTextEdit::keyPressEvent(e); }
    return;
  }
  // Determine if the <Tab> means completion:
  bool isShortcut = false;
  if ((e->modifiers() == Qt::NoModifier) && (e->key() == Qt::Key_Tab)) {
    isShortcut = (0 != textUnderCursor().length());
  }

  // If there is no completer, auto completion disabled or it is not a autocompletion task
  if(!_completer || !Preferences::get()->autoCompletion() || !isShortcut) {
    QTextEdit::keyPressEvent(e); return;
  }

  const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if ((! _completer) || (! Preferences::get()->autoCompletion())
    || (ctrlOrShift && e->text().isEmpty()))
  {
    return;
  }

  static QString eow("~!@#$%^&*()_+{}|:\"<>?,/;'[]\\-="); // end of word
  bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
  QString completionPrefix = textUnderCursor();

  if (!isShortcut && (hasModifier || e->text().isEmpty()||
                      completionPrefix.length() < Preferences::get()->autoCompletionThreshold()
                      || eow.contains(e->text().right(1))))
  {
    _completer->popup()->hide();
    return;
  }

  if (completionPrefix != _completer->completionPrefix())
  {
    _completer->setCompletionPrefix(completionPrefix);
    _completer->popup()->setCurrentIndex(_completer->completionModel()->index(0, 0));
  }

  QRect cr = cursorRect();
  cr.setWidth(_completer->popup()->sizeHintForColumn(0)
              + _completer->popup()->verticalScrollBar()->sizeHint().width());
  // popup it up!
  _completer->complete(cr);
}


void
CodeCell::focusInEvent(QFocusEvent *e)
{
  if (0 != _cell) { _cell->setActive(); }
  QTextEdit::focusInEvent(e);
}

void
CodeCell::focusOutEvent(QFocusEvent *e)
{
  if (0 != _cell) { _cell->setInactive(); }
  QTextEdit::focusOutEvent(e);
}

void
CodeCell::onCursorMoved() {
  if (0 == _cell) { return; }
  QTextCursor cursor = textCursor();
  cursor.movePosition(QTextCursor::StartOfLine);
  _cell->setSplitPosition(cursor.position());
}
