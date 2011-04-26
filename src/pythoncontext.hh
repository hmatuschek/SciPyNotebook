#ifndef PYTHONCONTEXT_HH
#define PYTHONCONTEXT_HH

#include <QObject>
#include <Python.h>


class PythonContext : public QObject
{
    Q_OBJECT

protected:
    PyObject *_globals;
    PyObject *_locals;

public:
    explicit PythonContext(QObject *parent = 0);

    PyObject *getGlobals();
    PyObject *getLocals();
};

#endif // PYTHONCONTEXT_HH
