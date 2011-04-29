#include "preferences.hh"
#include "configloader.hh"
#include <QDir>

Preferences * Preferences::_instance = 0;

Preferences::Preferences(QObject *parent) :
    QObject(parent)
{
  // Set path to config file:
  this->_configfile.setFileName(QDir::home().absoluteFilePath(".scipynotebookconfig"));

  // Set default settings for font
  this->_font = QFont("Fixed");

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



