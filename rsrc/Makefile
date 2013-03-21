SYSTEM_LIBRARY_DIR = /opt/local/lib
SYSTEM_BINARY_DIR = /opt/local/bin
QTCORE_FRAMEWORK = /opt/local/Library/Frameworks/QtCore.framework/Versions/4/QtCore
QTGUI_FRAMEWORK = /opt/local/Library/Frameworks/QtGui.framework/Versions/4/QtGui
QTXML_FRAMEWORK = /opt/local/Library/Frameworks/QtXml.framework/Versions/4/QtXml
QTSVG_FRAMEWORK = /opt/local/Library/Frameworks/QtSvg.framework/Versions/4/QtSvg
QTNET_FRAMEWORK = /opt/local/Library/Frameworks/QtNetwork.framework/Versions/4/QtNetwork
APP_LIBRARY_DIR = SciPyNotebook.app/Contents/Frameworks
APP_BINARY_DIR  = SciPyNotebook.app/Contents/MacOS
VERSION = 1.0.0rc2

app: dmg

clean:
	rm -f ${APP_BINARY_DIR}/scipynotebook
	rm -f ${APP_LIBRARY_DIR}/*
	rm -f SciPyNotebook-${VERSION}.dmg

dmg: scipynb-app
	hdiutil create -srcfolder SciPyNotebook.app SciPyNotebook-${VERSION}.dmg


scipynb-app: QtGui QtCore
	cp ../build/src/scipynotebook SciPyNotebook.app/Contents/MacOS
	install_name_tool -change ${QTCORE_FRAMEWORK} @executable_path/../Frameworks/QtCore ${APP_BINARY_DIR}/scipynotebook
	install_name_tool -change ${QTGUI_FRAMEWORK} @executable_path/../Frameworks/QtGui ${APP_BINARY_DIR}/scipynotebook

QtGui: QtCore libz libpng
	cp ${QTGUI_FRAMEWORK} ${APP_LIBRARY_DIR}
	install_name_tool -id @executable_path/../Frameworks/QtGui ${APP_LIBRARY_DIR}/QtGui
	install_name_tool -change ${QTCORE_FRAMEWORK} @executable_path/../Frameworks/QtCore ${APP_LIBRARY_DIR}/QtGui
	install_name_tool -change ${SYSTEM_LIBRARY_DIR}/libz.1.dylib @executable_path/../Frameworks/libz.1.dylib ${APP_LIBRARY_DIR}/QtGui
	install_name_tool -change ${SYSTEM_LIBRARY_DIR}/libpng15.15.dylib @executable_path/../Frameworks/libpng15.15.dylib ${APP_LIBRARY_DIR}/QtGui

QtCore: libz
	cp ${QTCORE_FRAMEWORK} ${APP_LIBRARY_DIR}
	install_name_tool -id @executable_path/../Frameworks/QtCore ${APP_LIBRARY_DIR}/QtCore
	install_name_tool -change ${SYSTEM_LIBRARY_DIR}/libz.1.dylib @executable_path/../Frameworks/libz.1.dylib ${APP_LIBRARY_DIR}/QtCore

libz:
	cp ${SYSTEM_LIBRARY_DIR}/libz.1.dylib ${APP_LIBRARY_DIR}
	install_name_tool -id @executable_path/../Frameworks/libz.1.dylib ${APP_LIBRARY_DIR}/libz.1.dylib

libpng: libz
	cp ${SYSTEM_LIBRARY_DIR}/libpng15.15.dylib ${APP_LIBRARY_DIR}
	install_name_tool -id @executable_path/../Frameworks/libpng15.15.dylib ${APP_LIBRARY_DIR}/libpng15.15.dylib
	install_name_tool -change ${SYSTEM_LIBRARY_DIR}/libz.1.dylib @executable_path/../Frameworks/libz.1.dylib ${APP_LIBRARY_DIR}/libpng15.15.dylib
