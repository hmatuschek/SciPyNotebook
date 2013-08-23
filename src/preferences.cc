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

#include "preferences.hh"

#include <QDir>
#include <QStringList>


Preferences * Preferences::_instance = 0;

Preferences::Preferences(QObject *parent) :
  QSettings("googlecode", "SciPyNotebook", parent)
{
  // Pass...
}


Preferences *
Preferences::get() {
  if(0 == Preferences::_instance) {
    Preferences::_instance = new Preferences();
  }
  return Preferences::_instance;
}


void
Preferences::setFont(const QFont &font) {
  // Store font:
  setValue("font", font);
  // And emit signal
  emit this->fontChanged();
}

QFont Preferences::font() const {
  return value("font", QFont("Courier")).value<QFont>();
}


void
Preferences::setPreamble(const QString &text) {
  setValue("preamble", text);
}


QString
Preferences::preamble() const {
  return value("preamble",
               "#\n# Default preamble\n#\n"
               "from numpy import *\n"
               "from scipy import *\n"
               "from matplotlib import pylab").toString();
}


int
Preferences::tabSize() {
  return value("tabsize", 4).toInt();
}


void
Preferences::setTabSize(int size) {
  setValue("tabsize", size);
}


bool
Preferences::autoCompletion() {
  return value("autocompletion", true).toBool();
}


void
Preferences::setAutoCompletion(bool enabled) {
  setValue("autocompletion", enabled);
}


int
Preferences::autoCompletionThreshold() {
  return value("autocompletionthreshold", 5).toInt();
}


void
Preferences::setAutoCompletionThreshold(int thres) {
  setValue("autocompletionthreshold", thres);
}


bool
Preferences::autoIndent() {
  return value("autoindent", true).toBool();
}

void
Preferences::setAutoIndent(bool enabled) {
  setValue("autoindent", enabled);
}

void
Preferences::addRecentFile(QString path) {
  QStringList recent_files = recentFiles();

  // Remove prior occurrence of path in recent_files:
  recent_files.removeAll(path); recent_files.prepend(path);
  while (recent_files.size() > 5) {
    recent_files.pop_back();
  }

  // Write recent files:
  beginWriteArray("recent");
  for (int i=0; i<recent_files.size(); i++) {
    setArrayIndex(i);
    setValue("path", recent_files.at(i));
  }
  endArray();
}

QStringList
Preferences::recentFiles() {
  QStringList recent_files;
  int size = beginReadArray("recent");
  for (int i=0; i<size; i++) {
    setArrayIndex(i);
    recent_files.append(value("path").toString());
  }
  endArray();
  return recent_files;
}
