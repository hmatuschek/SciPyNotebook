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

#ifndef PREFERENCES_HH
#define PREFERENCES_HH

#include <QObject>
#include <QFont>
#include <QFile>
#include <QSettings>

class Preferences : public QSettings
{
  Q_OBJECT

protected:
  static Preferences *_instance;

protected:
  QFont _font;
  QFile _configfile;
  QString _preamble;
  int _tabsize;

  bool _auto_completion;
  int _auto_completion_threshold;

protected:
  explicit Preferences(QObject *parent = 0);

public:  
  QFont font() const;
  QString preamble() const;
  int tabSize();

  bool autoCompletion();
  int autoCompletionThreshold();
  bool autoIndent();

public:
  static Preferences *get();

signals:
  void fontChanged();
  void tabSizeChanged();

public slots:
  void setFont(const QFont &font);
  void setPreamble(const QString &text);
  void setTabSize(int size);
  void setAutoCompletion(bool enabled);
  void setAutoCompletionThreshold(int thres);
  void setAutoIndent(bool enabled);
};

#endif // PREFERENCES_HH
