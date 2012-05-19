#-------------------------------------------------
#
# Project created by QtCreator 2012-05-19T15:07:37
#
#-------------------------------------------------

QT       -= gui

TARGET = scintilla
TEMPLATE = lib

DEFINES += SCINTILLA_LIBRARY

SOURCES += \
    ../src/ScintillaBase.cxx \
    ../src/RunStyles.cxx \
    ../src/RESearch.cxx \
    ../src/PositionCache.cxx \
    ../src/PerLine.cxx \
    ../src/LineMarker.cxx \
    ../src/KeyMap.cxx \
    ../src/Indicator.cxx \
    ../src/ExternalLexer.cxx \
    ../src/Editor.cxx \
    ../src/Document.cxx \
    ../src/Decoration.cxx \
    ../src/ContractionState.cxx \
    ../src/CharClassify.cxx \
    ../src/CellBuffer.cxx \
    ../src/Catalogue.cxx \
    ../src/CallTip.cxx \
    ../src/AutoComplete.cxx \
    ../src/XPM.cxx \
    ../src/ViewStyle.cxx \
    ../src/UniConversion.cxx \
    ../src/Style.cxx \
    ../src/Selection.cxx

HEADERS +=\
        scintilla_global.h \
    ../src/ScintillaBase.h \
    ../src/RunStyles.h \
    ../src/RESearch.h \
    ../src/PositionCache.h \
    ../src/PerLine.h \
    ../src/Partitioning.h \
    ../src/LineMarker.h \
    ../src/KeyMap.h \
    ../src/Indicator.h \
    ../src/FontQuality.h \
    ../src/ExternalLexer.h \
    ../src/Editor.h \
    ../src/Document.h \
    ../src/Decoration.h \
    ../src/ContractionState.h \
    ../src/CharClassify.h \
    ../src/CellBuffer.h \
    ../src/Catalogue.h \
    ../src/CallTip.h \
    ../src/AutoComplete.h \
    ../src/XPM.h \
    ../src/ViewStyle.h \
    ../src/UniConversion.h \
    ../src/SVector.h \
    ../src/Style.h \
    ../src/SplitVector.h \
    ../src/Selection.h

OTHER_FILES += \
    ../src/LexGen.py \
    ../src/SciTE.properties

