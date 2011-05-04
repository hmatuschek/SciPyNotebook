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

#include "configloader.hh"

ConfigLoader::ConfigLoader(Preferences *preferences) :
    QXmlDefaultHandler()
{
  // Store reference to preferences
  this->preferences = preferences;

  // Set initial state of parser;
  this->state_stack.push_back(START);
}


bool
ConfigLoader::startElement(const QString &namespaceURI, const QString &localName,
                           const QString &qName, const QXmlAttributes &attributes)
{
  if (START == this->state_stack.back())
  {
    if (localName == "SciPyNotebookConfig")
    {
      this->state_stack.push_back(PARSE_BODY);
      return true;
    }

    this->_error_string = QObject::tr("Unexpected element found: %1").arg(localName);
    return false;
  }


  if (PARSE_BODY == this->state_stack.back())
  {
    if (localName == "font")
    {
      this->state_stack.push_back(PARSE_FONT);
      return this->handleFont(attributes);
    }

    if (localName == "preamble")
    {
      this->state_stack.push_back(PARSE_PREAMBLE);
      return true;
    }

    if (localName == "tabsize")
    {
      this->state_stack.push_back(PARSE_TABSIZE);
      return true;
    }

    if (localName == "autocompletion")
    {
      this->state_stack.push_back(PARSE_AUTOCOMPLETION);
      return this->handleAutoCompletion(attributes);
    }

    this->_error_string = QObject::tr("Unexpected element found: %1").arg(localName);
    return false;
  }


  if (PARSE_FONT == this->state_stack.back() ||
      PARSE_PREAMBLE == this->state_stack.back() ||
      PARSE_TABSIZE == this->state_stack.back() ||
      PARSE_AUTOCOMPLETION == this->state_stack.back())
  {
    this->_error_string = QObject::tr("Unexpected element found: %1").arg(localName);
    return false;
  }

  this->_error_string = QObject::tr("Internal Error: Invalid parser state.");
  return false;
}


bool
ConfigLoader::endElement(const QString &namespaceURI, const QString &localName,
                         const QString &qName)
{
  // Simply pop state from stack:
  this->state_stack.pop_back();
  return true;
}


bool
ConfigLoader::startCDATA()
{
  if (PARSE_PREAMBLE == this->state_stack.back())
  {
    this->state_stack.push_back(PARSE_PREAMBLEDATA);
    return true;
  }

  this->_error_string = QObject::tr("Unexpected CDATA.");
  return false;
}


bool
ConfigLoader::endCDATA()
{
  this->state_stack.pop_back();
  return true;
}


bool
ConfigLoader::characters(const QString &str)
{
  if (PARSE_PREAMBLEDATA == this->state_stack.back())
  {
    this->preferences->setPreamble(str);
    return true;
  }

  if (PARSE_TABSIZE == this->state_stack.back())
  {
    bool ok;
    int tabsize = str.toInt(&ok);

    if ((! ok) || 0 > tabsize)
    {
      this->_error_string = QObject::tr("Invalid tab size %1").arg(str);
      return false;
    }

    this->preferences->setTabSize(tabsize);
    return true;
  }

  return true;
}


bool
ConfigLoader::fatalError(const QXmlParseException &exception)
{
  qWarning("Error while parsing config file: %s",
           exception.message().toStdString().c_str());

  return false;
}


bool
ConfigLoader::error(const QXmlParseException &exception)
{
  qWarning("Error while parsing config file: %s",
           exception.message().toStdString().c_str());

  return false;
}

bool
ConfigLoader::warning(const QXmlParseException &exception)
{
  qWarning("Warning while parsing config file: %s",
           exception.message().toStdString().c_str());

  return true;
}


QString
ConfigLoader::errorString() const
{
  return this->_error_string;
}


bool
ConfigLoader::handleFont(const QXmlAttributes &attributes)
{
  QFont font = this->preferences->font();

  // Handle font family attribute
  QString family = attributes.value("family");
  if (! family.isEmpty())
  {
    font.setFamily(family);
  }

  QString size = attributes.value("size");
  if (! size.isEmpty())
  {
    bool ok;
    int size_i = size.toInt(&ok);

    if (!ok || 0 > size_i)
    {
      this->_error_string = QObject::tr("Invalid font size: %1").arg(size);
      return false;
    }

    font.setPointSize(size_i);
  }

  this->preferences->setFont(font);
  return true;
}


bool
ConfigLoader::handleAutoCompletion(const QXmlAttributes &attributes)
{
  QString enabled = attributes.value("enabled");
  if("false" == enabled.toLower())
  {
    this->preferences->setAutoCompletion(false);
  }

  QString thres_str = attributes.value("threshold");
  if (!thres_str.isEmpty())
  {
    bool ok;
    int thres = thres_str.toInt(&ok);

    if (!ok || thres < 3)
    {
      this->_error_string = QObject::tr("Invalid threshold value %1").arg(thres_str);
      return false;
    }

    this->preferences->setAutoCompletionThreshold(thres);
  }

  return true;
}


