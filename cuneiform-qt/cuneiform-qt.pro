######################################################################
# Common settings

TEMPLATE = app
TARGET = cuneiform-qt
VERSION = 0.1.1

SOURCES += backend.cpp \
		   main.cpp \
           mainWindow.cpp \
		   settings.cpp

HEADERS += backend.h \
		   mainWindow.h \
		   settings.h

CONFIG += debug

CONFIG += release \
		  exceptions \
		  qt \
		  warn_on

RESOURCES = mainWindow.qrc

FORMS += mainWindow.ui \
		 settings.ui

PREFIX = $$(PREFIX)

isEmpty( PREFIX ) {
PREFIX = /usr/local
}

DATADIR = $$PREFIX/share/apps/cuneiform-qt/
TRANSLATIONS_DIR = $$DATADIR
isEmpty(NO_TRANSLATIONS):include("translations/translations.pri")

DEFINES += DATADIR=\\\"$$PREFIX/share/apps/cuneiform-qt/\\\"

target.path = $$PREFIX/bin/

icon.files = icons/cuneiform-qt.png
icon.path = $$PREFIX/share/pixmaps/

desktop.files = cuneiform-qt.desktop
desktop.path = $$PREFIX/share/applications/

INSTALLS += target \
			icon \
			desktop
