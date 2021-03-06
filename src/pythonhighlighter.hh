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

#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QLinkedList>


class HighlightingRule
{
public:
  typedef enum {
    DEFAULT = -1,
    QUOTATION_CONTINUATION = 1
  } State;

public:
  QRegExp pattern;
  QTextCharFormat format;
  int group;
  /** If this pattern matches, the current block gets this state. */
  State toState;
  /** Matches only if the last block is in state. */
  State inState;

  HighlightingRule(const QRegExp &p, const QTextCharFormat &f, int g=0, State is=DEFAULT, State ts=DEFAULT)
    : pattern(p), format(f), group(g), toState(ts), inState(is)
  {
    // Pass...
  }

  HighlightingRule(const HighlightingRule &other)
    : pattern(other.pattern), format(other.format), group(other.group), toState(other.toState), inState(other.inState)
  {
    // Pass...
  }
};


class PythonHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

protected:
    QLinkedList<HighlightingRule> rules;

    QFont defaultFont;
    QTextCharFormat keywordFormat;
    QTextCharFormat definitionFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat operatorFormat;


public:
    explicit PythonHighlighter(QTextEdit *parent = 0);


protected:
    virtual void highlightBlock(const QString &text);


signals:
public slots:
};

#endif // PYTHONHIGHLIGHTER_H
