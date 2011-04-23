#include "streamwrapper.hh"
#include "cellinputstream.hh"
#include <iostream>


static PyObject *
SciPyStudioStreamWrapper_write(SciPyStudioStreamWrapper *self, PyObject *args)
{
    PyObject *text = 0;

    // Get text to write from args
    if(!PyArg_ParseTuple(args, "S", &text))
    {
        std::cerr << "Invalid args..." << std::endl;
        return NULL;
    }

    std::cerr << "Redirect: " << text << std::endl;

    // Extract string and send it to stream:
    self->stream->write(PyString_AsString(text));

    // Return number of bytes send
    Py_RETURN_NONE;
}


static PyMethodDef SciPyStudioStreamWrapperType_methods[] =
{
    {"write", (PyCFunction)SciPyStudioStreamWrapper_write, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}
};



static PyTypeObject SciPyStudioStreamWrapperType = {
    PyObject_HEAD_INIT(NULL)
    0,
    "SciPyStudioStreamWrapper",
    sizeof(SciPyStudioStreamWrapperType),
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    Py_TPFLAGS_DEFAULT,
    "Wrapper of stderr and stdout.",
    0,0,0,0,0,0,
    SciPyStudioStreamWrapperType_methods,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


PyObject *
SciPyStudioStreamWrapper_new(CellInputStream *stream)
{
    SciPyStudioStreamWrapper *self;

    if (0 == (self = (SciPyStudioStreamWrapper *)SciPyStudioStreamWrapperType.tp_alloc(&SciPyStudioStreamWrapperType, 0)))
    {
        return 0;
    }

    // Init object
    self = (SciPyStudioStreamWrapper *)PyObject_Init((PyObject *)self, &SciPyStudioStreamWrapperType);

    // Store stream instance:
    self->stream = stream;

    return (PyObject *) self;
}


int
SciPyStudioStreamWrapperType_init()
{
    if (0 > PyType_Ready(&SciPyStudioStreamWrapperType))
    {
        return -1;
    }

    Py_INCREF(&SciPyStudioStreamWrapperType);

    return 0;
}
