
SOURCES += backend.cpp \
		   main.cpp \
           mainWindow.cpp \
		   settings.cpp

HEADERS += backend.h \
		   mainWindow.h \
		   settings.h

TEMPLATE = app

TRANSLATIONS = cuneiform-qt_ru.ts

CONFIG -= debug

CONFIG += release \
		  exceptions \
		  build_all \
		  qt \
		  warn_on

RESOURCES = mainWindow.qrc

FORMS += mainWindow.ui \
		 settings.ui

TARGET = cuneiform-qt

DESTDIR = $$(DESTDIR)
PREFIX = $$(PREFIX)

isEmpty( PREFIX ) {
PREFIX = /usr/local
}

DATADIR = $$DESTDIR/$$PREFIX/share/apps/cuneiform-qt/

DEFINES += DATADIR=\\\"$$PREFIX/share/apps/cuneiform-qt/\\\"

target.path = $$DESTDIR/$$PREFIX/bin/

translations.files = *.qm
translations.path = $$DATADIR

icons.files = images/cuneiform-qt.png
icons.path = $$DATADIR

INSTALLS += target
INSTALLS += translations 
INSTALLS += icons

