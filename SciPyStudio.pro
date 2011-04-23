#-------------------------------------------------
#
# Project created by QtCreator 2011-04-19T23:29:08
#
#-------------------------------------------------

QT       += core gui

TARGET = SciPyStudio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    codecell.cpp \
    pythonhighlighter.cpp \
    notebook.cpp \
    cell.cpp \
    resultcell.cc \
    cellstatus.cc \
    pythonengine.cc \
    cellinputstream.cc \
    streamwrapper.cc

HEADERS  += mainwindow.h \
    codecell.h \
    pythonhighlighter.h \
    notebook.h \
    cell.h \
    resultcell.hh \
    cellstatus.hh \
    pythonengine.hh \
    cellinputstream.hh \
    streamwrapper.hh

INCLUDEPATH += /opt/local/Library/Frameworks/Python.framework/Versions/2.6/include/python2.6
LIBS += -ldl -framework CoreFoundation -lpython2.6

FORMS    +=

OTHER_FILES += \
    README.rst
