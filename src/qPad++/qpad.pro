#-------------------------------------------------
#
# Project created by QtCreator 2012-06-01T12:51:33
#
#-------------------------------------------------

QT       += core gui network

TARGET = qpad
TEMPLATE = app

win32 {
    LIBS += ../../bin/libqscintilla2.a
}
!win32 {
    LIBS += -L../../bin/ -lqscintilla2
}

SOURCES += main.cpp\
    qnewmainwindow.cpp \
    base-class/basemainwindow.cpp \
    QtSingle/qtsinglecoreapplication.cpp \
    QtSingle/qtsingleapplication.cpp \
    QtSingle/qtlockedfile_win.cpp \
    QtSingle/qtlockedfile_unix.cpp \
    QtSingle/qtlockedfile.cpp \
    QtSingle/qtlocalpeer.cpp \
    base-class/basewidget.cpp \
    qpadmdiarea.cpp \
    qpadmdisubwindow.cpp

HEADERS  += qnewmainwindow.h \
    base-class/basemainwindow.h \
    inc/debug.h \
    inc/constant.h \
    inc/mem.h \
    QtSingle/qtsinglecoreapplication.h \
    QtSingle/qtsingleapplication.h \
    QtSingle/qtlockedfile.h \
    QtSingle/qtlocalpeer.h \
    res/strings.h \
    base-class/basewidget.h \
    inc/qpadlexer.h \
    qpadmdiarea.h \
    qpadmdisubwindow.h

FORMS    += ui/qnewmainwindow.ui

INCLUDEPATH += \
    QtSingle \
    inc \
    res \
    base-class \
    ../qscintilla-gpl-2.6.1/Qt4

TRANSLATIONS += \
    res/strings_en.ts

OTHER_FILES +=

DESTDIR = ../../bin

RESOURCES += \
    res/resource.qrc

release {
    DEFINES -= \
        _DEBUG_MODE
}

declarative_debug {
    DEFINES += \
        _DEBUG_MODE
}
