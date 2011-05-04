#ifndef PYTHONCOMPLETER_HH
#define PYTHONCOMPLETER_HH

#include <QCompleter>
#include "pythoncontext.hh"


class PythonCompleter : public QCompleter
{
    Q_OBJECT

protected:
  PythonContext *_context;
  QString _context_prefix;

public:
    explicit PythonCompleter(PythonContext *ctx, QObject *parent = 0);

    void setContextPrefix(const QString &prefix);

public slots:
    void setCompletionPrefix(const QString &prefix);
};

#endif // PYTHONCOMPLETER_HH
