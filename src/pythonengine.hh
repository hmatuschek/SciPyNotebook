#ifndef PYTHONENGINE_HH
#define PYTHONENGINE_HH

#include <QObject>

#include <Python.h>
#include "cellinputstream.hh"


class PythonEngine: public QObject
{
    Q_OBJECT;

protected:
    static PythonEngine *instance;
    PyObject *_locals;
    PyObject *_globals;

protected:
    explicit PythonEngine(QObject *parent=0);

public:
    static PythonEngine *get();

    ~PythonEngine();

    PyObject *getGlobals();
    PyObject *getLocals();

    void setStdout(CellInputStream *stream);
    void setStderr(CellInputStream *stream);
};



#endif // PYTHONENGINE_HH
