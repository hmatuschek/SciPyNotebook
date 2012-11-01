#include "pythoncompleter.hh"
#include <iostream>


PythonCompleter::PythonCompleter(PythonContext *ctx, QObject *parent) :
    QCompleter(parent), _context(ctx)
{
  _model = new QStringListModel();
  setModel(_model);
}


void
PythonCompleter::setCompletionPrefix(const QString &prefix)
{
  // Split prefix in object path and name prefix:
  QStringList names = prefix.split(".", QString::KeepEmptyParts);
  QString name_prefix = names.back(); names.pop_back();
  QString object_path = names.join(".");

  std::cerr << "Update list of objects from " << object_path.toStdString()
            << " with prefix " << name_prefix.toStdString() << std::endl;
  QStringList lst; _context->getNamesFor(object_path, lst);
  _model->setStringList(lst);
  QCompleter::setCompletionPrefix(name_prefix);
}
