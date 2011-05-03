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


CodeCell::CodeCell(QWidget *parent) :
    QTextEdit(parent)
{
    // Disable word-wrap
    this->setWordWrapMode(QTextOption::NoWrap);

    // install syntax highlighter
    this->higlighter = new PythonHighlighter(this);
    this->higlighter->setDocument(this->document());

    // Get default font from preferences:
    Preferences *prefs = Preferences::get();
    this->document()->setDefaultFont(prefs->font());

    // Set tab size:
    int tabsize = QFontMetrics(prefs->font()).width(' ');
    tabsize *= prefs->tabSize();
    this->setTabStopWidth(tabsize);

    // Layout specific stuff...
    this->_text_size = this->document()->size().toSize();
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QObject::connect(this, SIGNAL(textChanged()),
                     this, SLOT(onTextChanged()));

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setFrameShape(QFrame::NoFrame);

    // Set completer:
    this->_completer = 0;
}


CodeCell::~CodeCell()
{
    // destroy the syntax highlighter
    delete this->higlighter;
}



QSize
CodeCell::minimumSizeHint() const
{
    return this->document()->size().toSize();
}


QSize
CodeCell::sizeHint() const
{
    QSize csize(QTextEdit::sizeHint());
    csize.setHeight(this->document()->size().toSize().height());

    return csize;
}


void
CodeCell::onTextChanged()
{
  if (0 != this->extraSelections().size())
    this->clearLineMarks();
  this->_text_size = this->document()->size().toSize();
  this->updateGeometry();
}


void
CodeCell::markLine(size_t line)
{
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
  this->setExtraSelections( extras );
}


void
CodeCell::clearLineMarks()
{
  // just reset with empty extra selections
  QList<QTextEdit::ExtraSelection> extras;
  this->setExtraSelections(extras);
}


void
CodeCell::setCompleter(QCompleter *completer)
{
  if(this->_completer)
  {
    QObject::disconnect(this->_completer, 0, this, 0);
  }

  this->_completer = completer;

  if(! this->_completer)
    return;

  this->_completer->setWidget(this);
  this->_completer->setCompletionMode(QCompleter::PopupCompletion);
  this->_completer->setCaseSensitivity(Qt::CaseSensitive);
  QObject::connect(this->_completer, SIGNAL(activated(QString)),
                   this, SLOT(insertCompletion(QString)));
}


QCompleter *
CodeCell::completer()
{
  return this->_completer;
}


void
CodeCell::insertCompletion(const QString &completion)
{
  if (this->_completer->widget() != this)
           return;

  QTextCursor tc = this->textCursor();
  int extra = completion.length() - this->_completer->completionPrefix().length();
  tc.movePosition(QTextCursor::Left);
  tc.movePosition(QTextCursor::EndOfWord);
  tc.insertText(completion.right(extra));
  setTextCursor(tc);
}


QString
CodeCell::textUnderCursor()
{
  QTextCursor tc = this->textCursor();
  tc.select(QTextCursor::WordUnderCursor);
  return tc.selectedText();
}


void
CodeCell::keyPressEvent(QKeyEvent *e)
{
  if (this->_completer && this->_completer->popup()->isVisible())
  {
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

  bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
  if (!this->_completer || !isShortcut) // dont process the shortcut when we have a completer
    QTextEdit::keyPressEvent(e);

  const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if (!this->_completer || (ctrlOrShift && e->text().isEmpty()))
      return;

  static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
  bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
  QString completionPrefix = textUnderCursor();

  if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                    || eow.contains(e->text().right(1)))) {
      this->_completer->popup()->hide();
      return;
  }

  if (completionPrefix != this->_completer->completionPrefix()) {
      this->_completer->setCompletionPrefix(completionPrefix);
      this->_completer->popup()->setCurrentIndex(this->_completer->completionModel()->index(0, 0));
  }
  QRect cr = cursorRect();
  cr.setWidth(this->_completer->popup()->sizeHintForColumn(0)
              + this->_completer->popup()->verticalScrollBar()->sizeHint().width());
  this->_completer->complete(cr); // popup it up!
}
