#ifndef PYTHONCOMPLETER_HH
#define PYTHONCOMPLETER_HH

#include <QCompleter>
#include "pythoncontext.hh"


class PythonCompleter : public QCompleter
{
  Q_OBJECT

public:
  explicit PythonCompleter(PythonContext *ctx, QObject *parent = 0);

public slots:
  virtual void setCompletionPrefix(const QString &prefix);

private:
  PythonContext *_context;
  QStringListModel *_model;
};

#endif // PYTHONCOMPLETER_HH
