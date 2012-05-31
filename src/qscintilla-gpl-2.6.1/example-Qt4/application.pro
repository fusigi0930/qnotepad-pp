QT      += gui
CONFIG       += statics release
HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp
RESOURCES     = application.qrc
LIBS         += ../../../bin/libqscintilla2.a

INCLUDEPATH += ../Qt4
