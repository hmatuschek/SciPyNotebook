#
#
#

SET(scipynotebook_SOURCES cell.cc resultcell.cc
    cellinputstream.cc codecell.cc notebook.cc streamwrapper.cc
    cellstatus.cc pythonengine.cc main.cc pythonhighlighter.cc
    notebookwindow.cc aboutdialog.cc pythoncontext.cc preambledialog.cc
    preferencesdialog.cc preferences.cc configloader.cc)

SET(scipynotebook_HEADERS cell.hh resultcell.hh
    cellinputstream.hh notebook.hh cellstatus.hh
    pythonengine.hh codecell.hh pythonhighlighter.hh
    notebookwindow.hh aboutdialog.hh pythoncontext.hh preambledialog.hh
    preferencesdialog.hh preferences.hh configloader.hh)

QT4_WRAP_CPP(scipynotebook_HEADERS_MOC ${scipynotebook_HEADERS})


ADD_EXECUTABLE(scipynotebook ${scipynotebook_SOURCES} ${scipynotebook_HEADERS_MOC})
TARGET_LINK_LIBRARIES(scipynotebook ${QT_LIBRARIES} ${QT_QTXML_LIBRARY} ${PYTHON_LIBRARIES})

INSTALL(TARGETS scipynotebook
        DESTINATION bin)
