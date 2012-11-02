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

#include "pythonhighlighter.hh"
#include "preferences.hh"

#include <QTextEdit>
#include <iostream>



PythonHighlighter::PythonHighlighter(QTextEdit *parent) :
    QSyntaxHighlighter(parent), defaultFont(Preferences::get()->font())
{
    // Assemble rules for syntax highlighting (stolen from Scribus sources)
    // Reserved keywords in Python 2.4
    QStringList keywords;

    keywords << "and" << "assert" << "break" << "class" << "continue" << "def"
        << "del" << "elif" << "else" << "except" << "exec" << "finally"
        << "for" << "from" << "global" << "if" << "import" << "in"
        << "is" << "lambda" << "not" << "or" << "pass" << "print" << "raise"
        << "return" << "try" << "while" << "yield";

    /* Assemble formats. */
    keywordFormat.setFont(defaultFont);
    keywordFormat.setForeground(QColor(0x00, 0x00, 0x7f));
    keywordFormat.setFontWeight(QFont::Bold);

    definitionFormat.setFont(defaultFont);
    definitionFormat.setFontWeight(QFont::Bold);
    definitionFormat.setForeground(QColor(0x7F, 0x7F, 0x00));

    singleLineCommentFormat.setFont(defaultFont);
    singleLineCommentFormat.setForeground(QColor(0xA0, 0xA0, 0xA0));
    singleLineCommentFormat.setFontItalic(true);

    quotationFormat.setFont(defaultFont);
    //quotationFormat.setForeground(QColor(0x00, 0x55, 0x00));
    quotationFormat.setForeground(QColor(0x7f, 0x00, 0x00));

    numberFormat.setFont(defaultFont);
    numberFormat.setForeground(QColor(0xFF, 0xAA, 0x00));

    operatorFormat.setFont(defaultFont);
    operatorFormat.setForeground(QColor(0xAA, 0x00, 0xFF));

    /* Assemble rules */
    // Keywords
    rules.append(HighlightingRule(QRegExp("\\b(" + keywords.join("|") + ")\\b"), keywordFormat));
    // Numbers
    rules.append(HighlightingRule(QRegExp("\\b\\d+\\b"), numberFormat));
    // Operators
    rules.append(HighlightingRule(QRegExp("[\\\\|\\<|\\>|\\=|\\!|\\+|\\-|\\*|\\/|\\%]"), operatorFormat));
    // Comments
    rules.append(HighlightingRule(QRegExp("#.*$"), singleLineCommentFormat));
    // Single line strings:
    rules.append(HighlightingRule(QRegExp("\"[^\"]*\""), quotationFormat));
    rules.append(HighlightingRule(QRegExp("'[^']*'"), quotationFormat));
    // Multiline strings
    rules.append(
          HighlightingRule(
            QRegExp("\"[^\"]*$"), quotationFormat, 0,
            HighlightingRule::DEFAULT, HighlightingRule::QUOTATION_CONTINUATION));
    rules.append(
          HighlightingRule(
            QRegExp("^[^\"]*\""), quotationFormat, 0,
            HighlightingRule::QUOTATION_CONTINUATION, HighlightingRule::DEFAULT));
    rules.append(
          HighlightingRule(
          QRegExp("^[^\"]*$"), quotationFormat, 0,
            HighlightingRule::QUOTATION_CONTINUATION, HighlightingRule::QUOTATION_CONTINUATION));
}


void
PythonHighlighter::highlightBlock(const QString &text)
{
  std::cerr << "Hightlight '" << text.toStdString() <<
               "' in mode " << previousBlockState() << std::endl;

  // Set default font for all text:
  setFormat(0, text.length(), defaultFont);

  // Apply rules
  int currentIndex = 0;
  while (currentIndex < text.length()) {
    // Find first pattern matches
    int index = text.length(), length = 0;
    const HighlightingRule *matched_rule = 0;
    foreach (const HighlightingRule &rule, rules) {
      // Skip pattern with different block state
      if (previousBlockState() != rule.inState) { continue; }
      // If index matches:
      QRegExp expr(rule.pattern);
      if (0 > expr.indexIn(text, currentIndex)) { continue; }
      // Check if match is next:
      if ((index > expr.pos(rule.group)) || ((index == expr.pos(rule.group)) && (length < expr.cap(rule.group).size()))) {
        matched_rule = &rule;
        index = expr.pos(rule.group);
        length = expr.cap(rule.group).size();
      }
    }

    // If no rule matched -> done.
    if (0 == matched_rule) { return; }

    // Apply rule:
    setFormat(index, length, matched_rule->format);
    setCurrentBlockState(matched_rule->toState);

    // Update current start index:
    currentIndex = index+length;
  }
}
