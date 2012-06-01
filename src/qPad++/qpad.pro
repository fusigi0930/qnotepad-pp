#-------------------------------------------------
#
# Project created by QtCreator 2012-06-01T12:51:33
#
#-------------------------------------------------

QT       += core gui network

TARGET = qpad
TEMPLATE = app


SOURCES += main.cpp\
        qnewmainwindow.cpp \
    basemainwindow.cpp \
    QtSingle/qtsinglecoreapplication.cpp \
    QtSingle/qtsingleapplication.cpp \
    QtSingle/qtlockedfile_win.cpp \
    QtSingle/qtlockedfile_unix.cpp \
    QtSingle/qtlockedfile.cpp \
    QtSingle/qtlocalpeer.cpp

HEADERS  += qnewmainwindow.h \
    basemainwindow.h \
    debug.h \
    constant.h \
    mem.h \
    QtSingle/qtsinglecoreapplication.h \
    QtSingle/qtsingleapplication.h \
    QtSingle/qtlockedfile.h \
    QtSingle/qtlocalpeer.h

FORMS    += qnewmainwindow.ui

INCLUDEPATH += \
    QtSingle

OTHER_FILES +=

DESTDIR = ../../bin
