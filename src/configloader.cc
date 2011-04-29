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
    if (localName == "Font")
    {
      this->state_stack.push_back(PARSE_FONT);
      return this->handleFont(attributes);
    }

    this->_error_string = QObject::tr("Unexpected element found: %1").arg(localName);
    return false;
  }

  if (PARSE_FONT == this->state_stack.back())
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
ConfigLoader::characters(const QString &str)
{
  return true;
}


bool
ConfigLoader::fatalError(const QXmlParseException &exception)
{
  qWarning("Error while parsing config file: %s",
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
  QFont font = this->preferences->getFont();

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


