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

  // Save preamble:
  QDomElement preambleElement = document.createElement("preamble");
  QDomCDATASection preambleCode = document.createCDATASection(this->_preamble);
  preambleElement.appendChild(preambleCode);
  root.appendChild(preambleElement);

  // Open file for reading:
  if (! this->_configfile.open(QFile::WriteOnly | QFile::Truncate))
  {
    qWarning("Can not open config file for writing.");
    return;
  }

  // Save config:
  QTextStream out(&(this->_configfile));
  document.save(out, 4);
  qWarning(document.toString().toStdString().c_str());

  // Done.
  this->_configfile.close();
}



