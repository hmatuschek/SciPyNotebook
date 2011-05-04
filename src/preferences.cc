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
#include "configloader.hh"

#include <QDir>
#include <QDomDocument>
#include <QDomElement>


Preferences * Preferences::_instance = 0;

Preferences::Preferences(QObject *parent) :
    QObject(parent)
{
  // Set path to config file:
  this->_configfile.setFileName(QDir::home().absoluteFilePath(".scipynotebookconfig"));

  // Set default settings for font
  this->_font = QFont("Courier");

  // Set default settings for
  this->_tabsize = 4;

  // Set default for preamble:
  this->_preamble = "#\n# Default preamble\n#\n" \
                    "from numpy import *\n" \
                    "from scipy import *\n" \
                    "from matplotlib import pylab";

  // Auto completion is enabled by default
  this->_auto_completion = true;
  this->_auto_completion_threshold = 5;

  // Check if config file exists:
  if (! this->_configfile.exists())
  {
    qWarning("Config file \"%s\" does not exist yet.",
             this->_configfile.fileName().toStdString().c_str());

    // skip parsing:
    return;
  }

  // Setup XML parser
  ConfigLoader handler(this);
  QXmlSimpleReader reader;
  reader.setContentHandler(&handler);
  reader.setErrorHandler(&handler);
  reader.setLexicalHandler(&handler);

  // Open file
  if (! this->_configfile.open(QFile::ReadOnly | QFile::Text)) {
    qWarning("Can not open config file \"%s\".",
             this->_configfile.fileName().toStdString().c_str());

    // skip parsing
    return;
  }

  QXmlInputSource inputsource(&(this->_configfile));
  reader.parse(inputsource);

  this->_configfile.close();
}


Preferences *
Preferences::get()
{
  if(0 == Preferences::_instance)
  {
    Preferences::_instance = new Preferences();
  }

  return Preferences::_instance;
}


void
Preferences::setFont(const QFont &font)
{
  // Set font
  this->_font = font;

  // And emit signal
  emit this->fontChanged();
}


QFont
Preferences::font() const
{
  return this->_font;
}


void
Preferences::setPreamble(const QString &text)
{
  this->_preamble = text;
}


QString
Preferences::preamble() const
{
  return this->_preamble;
}


int
Preferences::tabSize()
{
  return this->_tabsize;
}


void
Preferences::setTabSize(int size)
{
  this->_tabsize = size;
}


bool
Preferences::autoCompletion()
{
  return this->_auto_completion;
}


void
Preferences::setAutoCompletion(bool enabled)
{
  this->_auto_completion = enabled;
}


int
Preferences::autoCompletionThreshold()
{
  return this->_auto_completion_threshold;
}


void
Preferences::setAutoCompletionThreshold(int thres)
{
  this->_auto_completion_threshold = thres;
}


void
Preferences::save()
{
  QDomDocument document;

  // Create document element:
  QDomElement root = document.createElement("SciPyNotebookConfig");
  root.setAttribute("version", "1.0");
  document.appendChild(root);

  // Save Font:
  QDomElement fontElement = document.createElement("font");
  fontElement.setAttribute("family", this->_font.family());
  fontElement.setAttribute("size", QString("%1").arg(this->font().pointSize()));
  root.appendChild(fontElement);

  // Save tab size:
  QDomElement tabElement = document.createElement("tabsize");
  QDomText tabValueElement = document.createTextNode(QString("%1").arg(this->_tabsize));
  tabElement.appendChild(tabValueElement);
  root.appendChild(tabElement);

  // Save preamble:
  QDomElement preambleElement = document.createElement("preamble");
  QDomCDATASection preambleCode = document.createCDATASection(this->_preamble);
  preambleElement.appendChild(preambleCode);
  root.appendChild(preambleElement);

  // Save autocompletion section:
  QDomElement autoComplElement = document.createElement("autocompletion");
  if (this->_auto_completion)
    autoComplElement.setAttribute("enabled", "true");
  else
    autoComplElement.setAttribute("enabled", "false");
  autoComplElement.setAttribute("threshold", this->_auto_completion_threshold);
  root.appendChild(autoComplElement);

  // Open file for writing:
  if (! this->_configfile.open(QFile::WriteOnly | QFile::Truncate))
  {
    qWarning("Can not open config file for writing.");
    return;
  }

  // Save config:
  QTextStream out(&(this->_configfile));
  document.save(out, 4);

  // Done.
  this->_configfile.close();
}



