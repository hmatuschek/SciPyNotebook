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



PythonHighlighter::PythonHighlighter(QTextEdit *parent) :
    QSyntaxHighlighter(parent), defaultFont(Preferences::get()->font())
{
    // Assemble rules for syntax highlighting (stolen from Scribus sources)
    // Reserved keywords in Python 2.4
    QStringList keywords;
    HighlightingRule rule;

    keywords << "and" << "assert" << "break" << "class" << "continue" << "def"
        << "del" << "elif" << "else" << "except" << "exec" << "finally"
        << "for" << "from" << "global" << "if" << "import" << "in"
        << "is" << "lambda" << "not" << "or" << "pass" << "print" << "raise"
        << "return" << "try" << "while" << "yield";

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
    quotationFormat.setForeground(QColor(0x00, 0x55, 0x00));

    numberFormat.setFont(defaultFont);
    numberFormat.setForeground(QColor(0xFF, 0xAA, 0x00));

    operatorFormat.setFont(defaultFont);
    operatorFormat.setForeground(QColor(0xAA, 0x00, 0xFF));

    rule.pattern = QRegExp("#[^\n]*");
    rule.format = singleLineCommentFormat;
    rule.group = 0;
    rules.append(rule);

    rule.pattern = QRegExp("\'.*\'");
    rule.pattern.setMinimal(true);
    rule.format = quotationFormat;
    rule.group = 0;
    rules.append(rule);

    rule.pattern = QRegExp("\".*\"");
    rule.pattern.setMinimal(true);
    rule.format = quotationFormat;
    rule.group = 0;
    rules.append(rule);

    foreach (QString kw, keywords) {
      rule.pattern = QRegExp("\\b" + kw + "\\b", Qt::CaseInsensitive);
      rule.format = keywordFormat;
      rule.group = 0;
      rules.append(rule);
    }

    rule.pattern = QRegExp("\\b(def|class)\\s+(\\w+)\\b");
    rule.format = definitionFormat;
    rule.group = 2;
    rules.append(rule);

    rule.pattern = QRegExp("\\b\\d+\\b");
    rule.pattern.setMinimal(true);
    rule.format = numberFormat;
    rule.group = 0;
    rules.append(rule);

    rule.pattern = QRegExp("[\\\\|\\<|\\>|\\=|\\!|\\+|\\-|\\*|\\/|\\%]+");
    rule.pattern.setMinimal(true);
    rule.format = operatorFormat;
    rule.group = 0;
    rules.append(rule);
}


void
PythonHighlighter::highlightBlock(const QString &text)
{
    // Set default font for all text:
    this->setFormat(0, text.length(), this->defaultFont);

    // Perform pattern matching (also stolen from Scribus sources)
    foreach (HighlightingRule rule, rules)
    {
        // compile pattern:
        QRegExp expression(rule.pattern);
        // match and get position of matched group
        expression.indexIn(text);
        int index = expression.pos(rule.group);

        while (index >= 0)
        {
            int length = expression.cap(rule.group).size();
            setFormat(index, length, rule.format);
            expression.indexIn(text, index + length);
            index = expression.pos(rule.group);
        }
    }

    setCurrentBlockState(0);

    // multiline strings handling
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf("\"\"\"");

    while (startIndex >= 0)
    {
        int endIndex = text.indexOf("\"\"\"", startIndex);
        int commentLength;

        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + 3;//commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, quotationFormat);
        startIndex = text.indexOf("\"\"\"", startIndex + commentLength);
    }
}
