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

#ifndef CONFIGLOADER_HH
#define CONFIGLOADER_HH

#include <QXmlDefaultHandler>
#include <QList>

#include "preferences.hh"

/**
 * \todo Rename to ConfigHandler
 */
class ConfigLoader : public QXmlDefaultHandler
{
protected:
  typedef enum {
    START,
    PARSE_BODY,
    PARSE_FONT,
    PARSE_PREAMBLE, PARSE_PREAMBLEDATA,
    PARSE_TABSIZE
  } State;

protected:
  /**
   * Holds a reference to the preferences instance.
   */
  Preferences *preferences;

  QList<State> state_stack;

  QString _error_string;


protected:
  bool handleFont(const QXmlAttributes &attributes);


public:
  ConfigLoader(Preferences *preferences);

  virtual bool startElement(const QString &namespaceURI, const QString &localName,
                            const QString &qName, const QXmlAttributes &attributes);

  virtual bool endElement(const QString &namespaceURI, const QString &localName,
                          const QString &qName);

  virtual bool startCDATA();
  virtual bool endCDATA();

  virtual bool characters(const QString &str);

  virtual bool fatalError(const QXmlParseException &exception);
  virtual bool error(const QXmlParseException &exception);
  virtual bool warning(const QXmlParseException &exception);

  virtual QString errorString() const;
};

#endif // CONFIGLOADER_HH
