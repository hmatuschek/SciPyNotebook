#include "preferences.hh"
#include "configloader.hh"
#include <QDir>

Preferences * Preferences::_instance = 0;

Preferences::Preferences(QObject *parent) :
    QObject(parent)
{
  // Set path to config file:
  this->configfile.setFileName(QDir::home().absoluteFilePath(".scipynotebookconfig"));

  // Set default settings for font
  this->font = QFont("Fixed");

  // Check if config file exists:
  if (! this->configfile.exists())
  {
    qWarning("Config file \"%s\" does not exist yet.",
             this->configfile.fileName().toStdString().c_str());

    // skip parsing:
    return;
  }

  // Setup XML parser
  ConfigLoader handler(this);
  QXmlSimpleReader reader;
  reader.setContentHandler(&handler);
  reader.setErrorHandler(&handler);

  // Open file
  if (! this->configfile.open(QFile::ReadOnly | QFile::Text)) {
    qWarning("Can not open config file \"%s\".",
             this->configfile.fileName().toStdString().c_str());

    // skip parsing
    return;
  }

  QXmlInputSource inputsource(&(this->configfile));
  reader.parse(inputsource);

  this->configfile.close();
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
Preferences::setFont(QFont &font)
{
  // Set font
  this->font = font;

  // And emit signal
  emit this->fontChanged();
}


QFont
Preferences::getFont() const
{
  return this->font;
}



