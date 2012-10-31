#include "pythoncompleter.hh"

PythonCompleter::PythonCompleter(PythonContext *ctx, QObject *parent) :
    QCompleter(parent), _context(ctx)
{
  _context_prefix = "";
  setModel(ctx->getNamesOf(_context_prefix));
}


void
PythonCompleter::setContextPrefix(const QString &prefix)
{
  // If context prefix changed -> update model;
  if (_context_prefix != prefix) {
    setModel(_context->getNamesOf(prefix));
  }
  // store prefix;
  _context_prefix = prefix;
}


void
PythonCompleter::setCompletionPrefix(const QString &prefix)
{
  QStringList names = prefix.split(".", QString::KeepEmptyParts);

  QString name = names.back();
  names.pop_back();

  QString context_prefix = names.join(".");

  setContextPrefix(context_prefix);
  QCompleter::setCompletionPrefix(name);
}
