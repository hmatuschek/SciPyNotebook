#include "pythonengine.hh"
#include <iostream>


PythonEngine *PythonEngine::instance = 0;


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
    return PyInt_FromSize_t(PyString_GET_SIZE(text));
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



PythonEngine::PythonEngine(QObject *parent) :
        QObject(parent)
{
    Py_Initialize();
    PythonEngine::instance = this;

    // Register SciPyStudioStreamWrapper class:
    if (0 > PyType_Ready(&SciPyStudioStreamWrapperType))
    {
        std::cerr << "Oops: Can not register stream wrapper..." << std::endl;
        exit(0);
    }
    Py_INCREF(&SciPyStudioStreamWrapperType);

    // Allcocate initial variable scopes (top-level)
    this->_locals = PyDict_New();
    this->_globals = PyDict_New();

    if (0 == this->_globals || 0 == this->_locals)
    {
        std::cerr << "Oops: Can not create scopes..." << std::endl;
        exit(0);
    }

    // Load __main__ module:
    PyObject *main_module = 0;
    if (0 == (main_module = PyImport_AddModule("__main__")))
    {
        std::cerr << "Opps: Can not import '__main__'" << std::endl;
        exit(-1);
    }

    // Populate local and global scope
    PyDict_Merge(this->_globals, PyModule_GetDict(main_module), 0);
    PyDict_Merge(this->_locals, this->_globals, 0);

    std::cerr << "Python interpreter started..." << std::endl;
}



PythonEngine::~PythonEngine()
{
    std::cerr << "Finalize python interpreter..." << std::endl;
    Py_Finalize();
    PythonEngine::instance = 0;
}


PythonEngine *
PythonEngine::get()
{
    if (0 != PythonEngine::instance)
    {
        return PythonEngine::instance;
    }

    PythonEngine::instance = new PythonEngine();
    return PythonEngine::instance;
}


PyObject *
PythonEngine::getGlobals()
{
    return this->_globals;
}


PyObject *
PythonEngine::getLocals()
{
    return this->_locals;
}


void
PythonEngine::setStdout(CellInputStream *stream)
{
    // Load sys:
    PyObject *sys_module = 0;
    if (0 == (sys_module = PyImport_AddModule("sys")))
    {
        std::cerr << "Opps: Can not import 'sys'" << std::endl;
        exit(-1);
    }

    PyObject *sys_dict = PyModule_GetDict(sys_module);
    PyDict_SetItem(sys_dict, PyString_FromString("stdout"), SciPyStudioStreamWrapper_new(stream));
}


void
PythonEngine::setStderr(CellInputStream *stream)
{
    // Load sys:
    PyObject *sys_module = 0;
    if (0 == (sys_module = PyImport_AddModule("sys")))
    {
        std::cerr << "Opps: Can not import 'sys'" << std::endl;
        exit(-1);
    }

    PyObject *sys_dict = PyModule_GetDict(sys_module);
    PyDict_SetItem(sys_dict, PyString_FromString("stderr"), SciPyStudioStreamWrapper_new(stream));
}

