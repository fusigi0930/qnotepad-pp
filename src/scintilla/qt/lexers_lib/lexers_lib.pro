#-------------------------------------------------
#
# Project created by QtCreator 2012-05-19T15:07:37
#
#-------------------------------------------------

QT       -= gui

TARGET = Lexers
TEMPLATE = lib
CONFIG += staticlib
DEFINES += SCINTILLA_LIBRARY

#QMAKE_CXXFLAGS += -EHsc -Zc:forScope -Zc:wchar_t -D_CRT_SECURE_NO_DEPRECATE=1

#QMAKE_LFLAGS += -Wl,--add-stdcall-alias -shared

INCLUDEPATH += \
	../../include \
	../../lexlib
	

SOURCES += \
    ../../lexlib/WordList.cxx \
    ../../lexlib/StyleContext.cxx \
    ../../lexlib/PropSetSimple.cxx \
    ../../lexlib/LexerSimple.cxx \
    ../../lexlib/LexerNoExceptions.cxx \
    ../../lexlib/LexerModule.cxx \
    ../../lexlib/LexerBase.cxx \
    ../../lexlib/CharacterSet.cxx \
    ../../lexlib/Accessor.cxx \

HEADERS +=\
    ../../lexlib/StyleContext.h \
    ../../lexlib/SparseState.h \
    ../../lexlib/PropSetSimple.h \
    ../../lexlib/OptionSet.h \
    ../../lexlib/LexerSimple.h \
    ../../lexlib/LexerNoExceptions.h \
    ../../lexlib/LexerModule.h \
    ../../lexlib/LexerBase.h \
    ../../lexlib/LexAccessor.h \
    ../../lexlib/CharacterSet.h \
    ../../lexlib/Accessor.h \
    ../../lexlib/WordList.h

# custom actions
win32 {
    CONFIG(release,debug|release) {
        QMAKE_POST_LINK += PostBuild.bat release
    } else {
        QMAKE_POST_LINK += PostBuild.bat debug
    }
} else {

}

