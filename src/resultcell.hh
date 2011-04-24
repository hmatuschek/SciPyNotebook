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

#ifndef RESULTCELL_HH
#define RESULTCELL_HH

#include <QTextEdit>
#include "cellinputstream.hh"


class ResultCell : public QTextEdit
{
    Q_OBJECT

private:
    QSize _text_size;
    CellInputStream _stdout_stream;
    CellInputStream _stderr_stream;

    QTextCharFormat stdoutFormat;
    QTextCharFormat stderrFormat;

public:
    explicit ResultCell(QWidget *parent = 0);

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;

    CellInputStream *getStdoutStream();
    CellInputStream *getStderrStream();


signals:


public slots:
    void onTextChanged();
    void onStdoutText(const QString &data);
    void onStderrText(const QString &data);
};

#endif // RESULTCELL_HH
