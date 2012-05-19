#-------------------------------------------------
#
# Project created by QtCreator 2012-05-19T15:07:37
#
#-------------------------------------------------

QT       -= gui

TARGET = scintilla
TEMPLATE = lib
CONFIG+=dll

DEFINES += SCINTILLA_LIBRARY

#QMAKE_CXXFLAGS += -Zi -TP -W4 -EHsc -Zc:forScope -Zc:wchar_t -D_CRT_SECURE_NO_DEPRECATE=1

QMAKE_LFLAGS += -Wl,--add-stdcall-alias -shared

INCLUDEPATH +=../src \
	../include \
	../lexlib
	

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
    ../src/Selection.cxx \
    ../lexlib/WordList.cxx \
    ../lexlib/StyleContext.cxx \
    ../lexlib/PropSetSimple.cxx \
    ../lexlib/LexerSimple.cxx \
    ../lexlib/LexerNoExceptions.cxx \
    ../lexlib/LexerModule.cxx \
    ../lexlib/LexerBase.cxx \
    ../lexlib/CharacterSet.cxx \
    ../lexlib/Accessor.cxx \
    ../lexers/LexYAML.cxx \
    ../lexers/LexVHDL.cxx \
    ../lexers/LexVerilog.cxx \
    ../lexers/LexVB.cxx \
    ../lexers/LexUser.cxx \
    ../lexers/LexTxt2tags.cxx \
    ../lexers/LexTeX.cxx \
    ../lexers/LexTCL.cxx \
    ../lexers/LexTAL.cxx \
    ../lexers/LexTADS3.cxx \
    ../lexers/LexTACL.cxx \
    ../lexers/LexSQL.cxx \
    ../lexers/LexSpice.cxx \
    ../lexers/LexSpecman.cxx \
    ../lexers/LexSorcus.cxx \
    ../lexers/LexSML.cxx \
    ../lexers/LexSmalltalk.cxx \
    ../lexers/LexSearchResult.cxx \
    ../lexers/LexScriptol.cxx \
    ../lexers/LexRuby.cxx \
    ../lexers/LexRebol.cxx \
    ../lexers/LexR.cxx \
    ../lexers/LexPython.cxx \
    ../lexers/LexPS.cxx \
    ../lexers/LexProgress.cxx \
    ../lexers/LexPowerShell.cxx \
    ../lexers/LexPowerPro.cxx \
    ../lexers/LexPOV.cxx \
    ../lexers/LexPLM.cxx \
    ../lexers/LexPerl.cxx \
    ../lexers/LexPB.cxx \
    ../lexers/LexPascal.cxx \
    ../lexers/LexOthers.cxx \
    ../lexers/LexOpal.cxx \
    ../lexers/LexObjC.cxx \
    ../lexers/LexNsis.cxx \
    ../lexers/LexNimrod.cxx \
    ../lexers/LexMySQL.cxx \
    ../lexers/LexMSSQL.cxx \
    ../lexers/LexMPT.cxx \
    ../lexers/LexModula.cxx \
    ../lexers/LexMMIXAL.cxx \
    ../lexers/LexMetapost.cxx \
    ../lexers/LexMatlab.cxx \
    ../lexers/LexMarkdown.cxx \
    ../lexers/LexMagik.cxx \
    ../lexers/LexLua.cxx \
    ../lexers/LexLout.cxx \
    ../lexers/LexLisp.cxx \
    ../lexers/LexKix.cxx \
    ../lexers/LexInno.cxx \
    ../lexers/LexHTML.cxx \
    ../lexers/LexHaskell.cxx \
    ../lexers/LexGui4Cli.cxx \
    ../lexers/LexGAP.cxx \
    ../lexers/LexFortran.cxx \
    ../lexers/LexForth.cxx \
    ../lexers/LexFlagship.cxx \
    ../lexers/LexEScript.cxx \
    ../lexers/LexErlang.cxx \
    ../lexers/LexEiffel.cxx \
    ../lexers/LexD.cxx \
    ../lexers/LexCSS.cxx \
    ../lexers/LexCsound.cxx \
    ../lexers/LexCrontab.cxx \
    ../lexers/LexCPP.cxx \
    ../lexers/LexConf.cxx \
    ../lexers/LexCOBOL.cxx \
    ../lexers/LexCmake.cxx \
    ../lexers/LexCLW.cxx \
    ../lexers/LexCaml.cxx \
    ../lexers/LexBullant.cxx \
    ../lexers/LexBasic.cxx \
    ../lexers/LexBash.cxx \
    ../lexers/LexBaan.cxx \
    ../lexers/LexAVE.cxx \
    ../lexers/LexAU3.cxx \
    ../lexers/LexASY.cxx \
    ../lexers/LexAsn1.cxx \
    ../lexers/LexAsm.cxx \
    ../lexers/LexAPDL.cxx \
    ../lexers/LexAda.cxx \
    ../lexers/LexAbaqus.cxx \
    ../lexers/LexA68k.cxx

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
    ../src/Selection.h \
    ../lexlib/StyleContext.h \
    ../lexlib/SparseState.h \
    ../lexlib/PropSetSimple.h \
    ../lexlib/OptionSet.h \
    ../lexlib/LexerSimple.h \
    ../lexlib/LexerNoExceptions.h \
    ../lexlib/LexerModule.h \
    ../lexlib/LexerBase.h \
    ../lexlib/LexAccessor.h \
    ../lexlib/CharacterSet.h \
    ../lexlib/Accessor.h \
    ../lexlib/WordList.h

OTHER_FILES += \
    ../src/LexGen.py \
    ../src/SciTE.properties

