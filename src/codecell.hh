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

#ifndef CODECELL_H
#define CODECELL_H

#include <QPlainTextEdit>
#include <QShortcut>
#include <QCompleter>

#include "pythonhighlighter.hh"


class CodeCell : public QTextEdit
{
    Q_OBJECT

protected:
    PythonHighlighter *higlighter;
    QSize _text_size;
    QCompleter *_completer;


protected:
    QString textUnderCursor();

    virtual void keyPressEvent(QKeyEvent *e);


public:
    explicit CodeCell(QWidget *parent = 0);
    ~CodeCell();

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;

    void setCompleter(QCompleter *completer);
    QCompleter *completer();


public slots:
    void onTextChanged();

    void markLine(size_t line);
    void clearLineMarks();


protected slots:
    void insertCompletion(const QString &completion);
};

#endif // CODECELL_H
