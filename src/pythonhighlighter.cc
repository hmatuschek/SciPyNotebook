/*
 * This file is part of the SciPyNotebook project.
 *
 * (c) 2011 Hannes Matuschek <hmatuschek AT gmail DOT com>
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

#include "pythonhighlighter.hh"
#include <QTextEdit>



PythonHighlighter::PythonHighlighter(QTextEdit *parent) :
    QSyntaxHighlighter(parent), defaultFont("Fixed")
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

    defaultFont.setStyleHint(QFont::TypeWriter);

    keywordFormat.setFont(defaultFont);
    keywordFormat.setForeground(QColor(0x00, 0x00, 0x7f));
    keywordFormat.setFontWeight(QFont::Bold);

    singleLineCommentFormat.setFont(defaultFont);
    singleLineCommentFormat.setForeground(QColor(0xA0, 0xA0, 0xA0));
    singleLineCommentFormat.setFontItalic(true);

    quotationFormat.setFont(defaultFont);
    quotationFormat.setForeground(QColor(0x00, 0x55, 0x00));

    numberFormat.setFont(defaultFont);
    numberFormat.setForeground(QColor(0xFF, 0xAA, 0x00));

    operatorFormat.setFont(defaultFont);
    operatorFormat.setForeground(QColor(0xAA, 0x00, 0xFF));

    foreach (QString kw, keywords)
    {
        rule.pattern = QRegExp("\\b" + kw + "\\b", Qt::CaseInsensitive);
        rule.format = keywordFormat;
        this->rules.append(rule);
    }

    rule.pattern = QRegExp("#[^\n]*");
    rule.format = singleLineCommentFormat;
    rules.append(rule);

    rule.pattern = QRegExp("\'.*\'");
    rule.pattern.setMinimal(true);
    rule.format = quotationFormat;
    rules.append(rule);

    rule.pattern = QRegExp("\".*\"");
    rule.pattern.setMinimal(true);
    rule.format = quotationFormat;
    rules.append(rule);

    rule.pattern = QRegExp("\\b\\d+\\b");
    rule.pattern.setMinimal(true);
    rule.format = numberFormat;
    rules.append(rule);

    rule.pattern = QRegExp("[\\\\|\\<|\\>|\\=|\\!|\\+|\\-|\\*|\\/|\\%]+");
    rule.pattern.setMinimal(true);
    rule.format = operatorFormat;
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
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
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
