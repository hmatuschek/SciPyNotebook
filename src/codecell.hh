/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011, 2012 Hannes Matuschek <hmatuschek AT gmail DOT com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef CODECELL_H
#define CODECELL_H

#include <QPlainTextEdit>
#include <QShortcut>
#include <QCompleter>
#include "pythonhighlighter.hh"
#include "cell.hh"

/**
 * Implements the part of the cell that shows the python-code.
 *
 * Implements syntax highlighting, highlighting of exception origin and
 * the auto-completion feature.
 */
class CodeCell : public QTextEdit
{
  Q_OBJECT

public:
  /** Constructs a new and empty code-cell. */
  explicit CodeCell(Cell *cell, QWidget *parent = 0);

  ~CodeCell();

  virtual QSize minimumSizeHint() const;
  virtual QSize sizeHint() const;


signals:
  void activated(CodeCell *cell);
  void inactivated(CodeCell *cell);
  void activateNextCell();
  void activatePrevCell();

public slots:
  void onTextChanged();
  void markLine(size_t line);
  void clearLineMarks();

private slots:
  void insertCompletion(const QString &completion);
  void onCellDeleted();

private:
  /** Returns the word under the cursor or "" if there is none. */
  QString textUnderCursor();

  void moveToStartOfWord(QTextCursor &cursor);
  void moveToEndOfWord(QTextCursor &cursor);

  /** Handles key-press event to show and updated auto-completion. */
  virtual void keyPressEvent(QKeyEvent *e);
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);

private:
  /** Holds the reference (and ownership) of the python highlighter. */
  PythonHighlighter *_higlighter;
  /** Holds a weak reference to the completer for auto-completion. */
  PythonCompleter *_completer;
  /** Internal used variable to hold the height of the text in pixel. Is needed to resize the
   * view to avoid scrolling. */
  QSize _text_size;
  /** A weak reference to the cell. */
  Cell *_cell;
};

#endif // CODECELL_H
