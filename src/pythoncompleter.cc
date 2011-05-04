#include "pythoncompleter.hh"

PythonCompleter::PythonCompleter(PythonContext *ctx, QObject *parent) :
    QCompleter(parent), _context(ctx)
{
  this->_context_prefix = "";
  this->setModel(ctx->getNamesOf(this->_context_prefix));
}


void
PythonCompleter::setContextPrefix(const QString &prefix)
{
  // If context prefix changed -> update model;
  if (this->_context_prefix != prefix)
  {
    this->setModel(this->_context->getNamesOf(prefix));
  }

  // store prefix;
  this->_context_prefix = prefix;
}


void
PythonCompleter::setCompletionPrefix(const QString &prefix)
{
  QStringList names = prefix.split(".", QString::KeepEmptyParts);

  QString name = names.back();
  names.pop_back();

  QString context_prefix = names.join(".");

  this->setContextPrefix(context_prefix);
  QCompleter::setCompletionPrefix(name);
}
