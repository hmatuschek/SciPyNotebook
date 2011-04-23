#ifndef STREAMWRAPPER_HH
#define STREAMWRAPPER_HH

#include <Python.h>
#include "cellinputstream.hh"


int SciPyStudioStreamWrapperType_init();

PyObject *SciPyStudioStreamWrapper_new(CellInputStream *stream);

typedef struct {
    PyObject_HEAD

    CellInputStream *stream;
} SciPyStudioStreamWrapper;


#endif // STREAMWRAPPER_HH
