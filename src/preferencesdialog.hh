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

#ifndef PREFERENCESDIALOG_HH
#define PREFERENCESDIALOG_HH

#include <QDialog>
#include <QFontComboBox>
#include <QSpinBox>
#include <QTextEdit>



class PreferencesDialog : public QDialog
{
  Q_OBJECT

protected:
  QFontComboBox *_fontBox;
  QSpinBox *_fontSizeBox;
  QTextEdit *_preambleBox;
  QSpinBox *_tabSizeBox;


public:
    explicit PreferencesDialog(QWidget *parent = 0);

    QFont font();
    QString preamble();
    int tabSize();
};

#endif // PREFERENCESDIALOG_HH
