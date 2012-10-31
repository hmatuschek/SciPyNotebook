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

#include "preferences.hh"

#include <QDir>


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


