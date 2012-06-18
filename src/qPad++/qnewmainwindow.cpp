#include "qnewmainwindow.h"
#include "ui_qnewmainwindow.h"
#include "debug.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <Qsci/qsciscintilla.h>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>
#include <QThread>
#include "strings.h"
#include "qpadlexer.h"
#include "constant.h"
#include <QTabBar>
#include "qpadmdisubwindow.h"
#include <QKeyEvent>
#include <Scintilla.h>
#include <SciLexer.h>
#include <Qsci/qsciprinter.h>
#include <QPrintDialog>
#include "qpadgotolinedialog.h"
#include <QTextCodec>
#include <QByteArray>
#include <QList>
#include <QListWidget>
#include "qpaddockfindresultwidget.h"
#include "qpadfindreplacedialog.h"

QNewMainWindow::QNewMainWindow(QWidget *parent) :
    BaseMainWindow(parent),
    ui(new Ui::QNewMainWindow),
    m_langActionsGroup(this),
    m_encodingActionsGroup(this)
{
    m_pMdiArea=NULL;
    m_nNewDocNum=1;
    m_vtMenuLangActions.clear();
    m_pDockFindResult=NULL;
    m_pFindDlg=NULL;

}

QNewMainWindow::~QNewMainWindow()
{
    _DEL_MEM(ui);

    for(QMap<unsigned int, QShortcut*>::iterator pFind=m_mapShortcuts.begin(); pFind != m_mapShortcuts.end(); ++pFind) {
        _DEL_MEM(pFind.value());
    }
    m_mapShortcuts.clear();

    m_pMdiArea->closeAllSubWindows();
    _DEL_MEM(m_pMdiArea);
    _DEL_MEM(m_pDockFindResult);
}

void QNewMainWindow::setMenuActions() {
    setFileMenuActions();
    setEditMenuActions();
    setLangMenuActions();
    setSearchMenuActions();
    setViewMenuActions();
    setEncodingActions();
    setSettingsActions();
    setMacroActions();
    setRunActions();
    setHelpActions();
}

void QNewMainWindow::setUnmenuActoins() {
//    QShortcut *p=new QShortcut(QKeySequence("Ctrl+F2"), this);
//    connect(p, SIGNAL(activated()), this, SLOT(actionUnmenuBookmark()));
//    m_mapShortcuts[EHT_BOOKMARKS]=p;

//    p=new QShortcut(QKeySequence("F2"), this);
//    connect(p, SIGNAL(activated()), this, SLOT(actionUnmenuBookmark()));
}

void QNewMainWindow::setFileMenuActions() {
    // File menu actions
    ui->actionFILE_NEW->setShortcuts(QKeySequence::New);
    connect(ui->actionFILE_NEW, SIGNAL(triggered()), this, SLOT(actionFileNew()));

    ui->actionFILE_OPEN->setShortcuts(QKeySequence::Open);
    connect(ui->actionFILE_OPEN, SIGNAL(triggered()), this, SLOT(actionFileOpen()));

    ui->actionFILE_SAVE->setShortcuts(QKeySequence::Save);
    connect(ui->actionFILE_SAVE, SIGNAL(triggered()), this, SLOT(actionFileSave()));

    connect(ui->actionFILE_RELOAD, SIGNAL(triggered()), this, SLOT(actionFileReload()));

    ui->actionFILE_SAVEAS->setShortcuts(QKeySequence::SaveAs);
    connect(ui->actionFILE_SAVEAS, SIGNAL(triggered()), this, SLOT(actionFileSaveAs()));

    connect(ui->actionFILE_SAVE_COPY, SIGNAL(triggered()), this, SLOT(actionFileSaveAsCopy()));

    connect(ui->actionFILE_SAVE_ALL, SIGNAL(triggered()), this, SLOT(actionFileSaveAll()));

    connect(ui->actionFILE_CLOSE, SIGNAL(triggered()), this, SLOT(actionFileClose()));

    connect(ui->actionFILE_CLOSE_ALL, SIGNAL(triggered()), this, SLOT(actionFileCloseAll()));

    connect(ui->actionFILE_CLOSE_EXCEPT_CUR, SIGNAL(triggered()), this, SLOT(actionFileCloseAllExceptCurrent()));

    connect(ui->actionFILE_EXIT, SIGNAL(triggered()), this, SLOT(actionFileExit()));

    connect(ui->actionFILE_RENAME, SIGNAL(triggered()), this, SLOT(actionFileRename()));

    ui->actionFILE_PRINT->setShortcuts(QKeySequence::Print);
    connect(ui->actionFILE_PRINT, SIGNAL(triggered()), this, SLOT(actionFilePrint()));

    connect(ui->actionFILE_PRINT_NOW, SIGNAL(triggered()), this, SLOT(actionFilePrintNow()));

    ui->actionFILE_DEL->setEnabled(false);
    ui->actionFILE_LOAD_SESSION->setEnabled(false);
    ui->actionFILE_SAVE_SESSION->setEnabled(false);
}

void QNewMainWindow::setEditMenuActions() {
    ui->actionEDIT_UNDO->setShortcuts(QKeySequence::Undo);
    connect(ui->actionEDIT_UNDO, SIGNAL(triggered()), this, SLOT(actionEditUndo()));

    ui->actionEDIT_REDO->setShortcuts(QKeySequence::Redo);
    connect(ui->actionEDIT_REDO, SIGNAL(triggered()), this, SLOT(actionEditRedo()));

    ui->actionEDIT_CUT->setShortcuts(QKeySequence::Cut);
    connect(ui->actionEDIT_CUT, SIGNAL(triggered()), this, SLOT(actionEditCut()));

    ui->actionEDIT_COPY->setShortcuts(QKeySequence::Copy);
    connect(ui->actionEDIT_COPY, SIGNAL(triggered()), this, SLOT(actionEditCopy()));

    ui->actionEDIT_PASTE->setShortcuts(QKeySequence::Paste);
    connect(ui->actionEDIT_PASTE, SIGNAL(triggered()), this, SLOT(actionEditPaste()));

    ui->actionEDIT_DEL->setShortcuts(QKeySequence::Delete);
    connect(ui->actionEDIT_DEL, SIGNAL(triggered()), this, SLOT(actionEditDel()));

    ui->actionEDIT_SELECT_ALL->setShortcuts(QKeySequence::SelectAll);
    connect(ui->actionEDIT_SELECT_ALL, SIGNAL(triggered()), this, SLOT(actionEditSelectAll()));

    ui->actionEDIT_COPY_CLIP_FULLPATH->setEnabled(false);
    ui->actionEDIT_COPY_CLIP_FILENAME->setEnabled(false);
    ui->actionEDIT_COPY_CLIP_DIRPATH->setEnabled(false);
    ui->actionEDIT_INDENT_INC->setEnabled(false);
    ui->actionEDIT_INDENT_DEC->setEnabled(false);
    ui->actionEDIT_CONVERT_CASE_UP->setEnabled(false);
    ui->actionEDIT_CONVERT_CASE_LOW->setEnabled(false);
    ui->actionEDIT_LINE_DUP->setEnabled(false);
    ui->actionEDIT_LINE_SPLIT->setEnabled(false);
    ui->actionEDIT_LINE_JOIN->setEnabled(false);
    ui->actionEDIT_LINE_UP->setEnabled(false);
    ui->actionEDIT_LINE_DOWN->setEnabled(false);
    ui->actionEDIT_COMMENT_TOGGLE_BLOCK->setEnabled(false);
    ui->actionEDIT_COMMENT_COMMENT->setEnabled(false);
    ui->actionEDIT_COMMENT_UNCOMMENT->setEnabled(false);
    ui->actionEDIT_COMMENT_STREAM->setEnabled(false);
    ui->actionEDIT_AUTO_COMPLETE_FUNC->setEnabled(false);
    ui->actionEDIT_AUTO_COMPLETE_WORD->setEnabled(false);
    ui->actionEDIT_AUTO_COMPLETE_PARAM->setEnabled(false);
    ui->actionEDIT_EOL_WINDOW->setEnabled(false);
    ui->actionEDIT_EOL_UNIX->setEnabled(false);
    ui->actionEDIT_EOL_MAC->setEnabled(false);
    ui->actionEDIT_BLANK_TRIM_TIAL->setEnabled(false);
    ui->actionEDIT_BLANK_TRIM_LEAD->setEnabled(false);
    ui->actionEDIT_BLANK_TRIM_BOTH->setEnabled(false);
    ui->actionEDIT_BLANK_EOL_SPACE->setEnabled(false);
    ui->actionEDIT_BLANK_TRIM_ALL->setEnabled(false);
    ui->actionEDIT_BLANK_TAP2SPACE->setEnabled(false);
    ui->actionEDIT_BLANK_SPACE2TAP->setEnabled(false);
    ui->actionEDIT_PASTE_HTML->setEnabled(false);
    ui->actionEDIT_PASTE_RTF->setEnabled(false);
    ui->actionEDIT_COPY_BINARY->setEnabled(false);
    ui->actionEDIT_CUT_BINARY->setEnabled(false);
    ui->actionEDIT_PASTE_BINARY->setEnabled(false);
    ui->actionEDIT_COLUMN_MODE->setEnabled(false);
    ui->actionEDIT_COLUMN_EDITOR->setEnabled(false);
    ui->actionEDIT_CHAR_PANEL->setEnabled(false);
    ui->actionEDIT_CLIP_HISTORY->setEnabled(false);
    ui->actionEDIT_READ_ONLY->setEnabled(false);
    ui->actionEDIT_UNREAD_ONLY->setEnabled(false);
}

void QNewMainWindow::setSearchMenuActions() {
    QList<QKeySequence> list;
    list.push_back(QKeySequence("Ctrl+F2"));
    ui->actionSEARCH_BOOKMARK_TOGGLE->setShortcuts(list);
    list.clear();
    connect(ui->actionSEARCH_BOOKMARK_TOGGLE, SIGNAL(triggered()), this, SLOT(actionSearchBookmark()));

    list.push_back(QKeySequence("F2"));
    ui->actionSEARCH_BOOKMARK_NEXT->setShortcuts(list);
    list.clear();
    connect(ui->actionSEARCH_BOOKMARK_NEXT, SIGNAL(triggered()), this, SLOT(actionSearchBookmarkNext()));

    list.push_back(QKeySequence("Shift+F2"));
    ui->actionSEARCH_BOOKMARK_PREV->setShortcuts(list);
    list.clear();
    connect(ui->actionSEARCH_BOOKMARK_PREV, SIGNAL(triggered()), this, SLOT(actionSearchBookmarkPrev()));

    list.push_back(QKeySequence("Ctrl+G"));
    ui->actionSEARCH_GOTO_LINE->setShortcuts(list);
    list.clear();
    connect(ui->actionSEARCH_GOTO_LINE, SIGNAL(triggered()), this, SLOT(actionSearchGotoLine()));

    list.push_back(QKeySequence("Ctrl+F"));
    ui->actionSEARCH_FIND->setShortcuts(list);
    list.clear();
    connect(ui->actionSEARCH_FIND, SIGNAL(triggered()), this, SLOT(actionSearchFind()));

    ui->actionSEARCH_FINDINFILES->setEnabled(false);
    ui->actionSEARCH_FIND_NEXT->setEnabled(false);
    ui->actionSEARCH_FIND_PREV->setEnabled(false);
    ui->actionSEARCH_FIND_SET_NEXt->setEnabled(false);
    ui->actionSEARCH_FIND_SET_PREV->setEnabled(false);
    ui->actionSEARCH_FIND_VOLITALE_NEXT->setEnabled(false);
    ui->actionSEARCH_FIND_VOLITALE_PREV->setEnabled(false);
    ui->actionSEARCH_REPLACE->setEnabled(false);
    ui->actionSEARCH_FIND_INC->setEnabled(false);
    ui->actionSEARCH_FIND_RESULT->setEnabled(false);
    ui->actionSEARCH_GOTO_NEXT_FIND->setEnabled(false);
    ui->actionSEARCH_GOTO_PREV_FIND->setEnabled(false);

    ui->actionSEARCH_GOTO_MATCH_BRACE->setEnabled(false);
    ui->actionSEARCH_MARKALL_1->setEnabled(false);
    ui->actionSEARCH_MARKALL_2->setEnabled(false);
    ui->actionSEARCH_MARKALL_3->setEnabled(false);
    ui->actionSEARCH_MARKALL_4->setEnabled(false);
    ui->actionSEARCH_MARK_ALL_5->setEnabled(false);
    ui->actionSEARCH_UNMARKALL_1->setEnabled(false);
    ui->actionSEARCH_UNMARKALL_2->setEnabled(false);
    ui->actionSEARCH_UNMARKALL_3->setEnabled(false);
    ui->actionSEARCH_UNMARKALL_4->setEnabled(false);
    ui->actionSEARCH_UNMARKALL_5->setEnabled(false);
    ui->actionSEARCH_UNMARKALL_ALL->setEnabled(false);
    ui->actionSEARCH_JUMP_PREV_1->setEnabled(false);
    ui->actionSEARCH_JUMP_PREV_2->setEnabled(false);
    ui->actionSEARCH_JUMP_PREV_3->setEnabled(false);
    ui->actionSEARCH_JUMP_PREV_4->setEnabled(false);
    ui->actionSEARCH_JUMP_PREV_5->setEnabled(false);
    ui->actionSEARCH_JUMP_PREV_DEF->setEnabled(false);
    ui->actionSEARCH_JUMP_NEXT_1->setEnabled(false);
    ui->actionSEARCH_JUMP_NEXT_2->setEnabled(false);
    ui->actionSEARCH_JUMP_NEXT_3->setEnabled(false);
    ui->actionSEARCH_JUMP_NEXT_4->setEnabled(false);
    ui->actionSEARCH_JUMP_NEXT_5->setEnabled(false);
    ui->actionSEARCH_JUMP_NEXT_DEF->setEnabled(false);
    ui->actionSEARCH_BOOKMARK_CLEAR->setEnabled(false);
    ui->actionSEARCH_BOOKMARK_CUT_LINE->setEnabled(false);
    ui->actionSEARCH_BOOKMARK_COPY_LINE->setEnabled(false);
    ui->actionSEARCH_BOOKMARK_PASTE_LINE->setEnabled(false);
    ui->actionSEARCH_BOOKMARK_DEL_LINE->setEnabled(false);
    ui->actionSEARCH_BOOKMARK_DEL_UNMARK_LINE->setEnabled(false);
    ui->actionSEARCH_BOOKMARK_INVERSE_BOOKMARK->setEnabled(false);
    ui->actionSEARCH_FIND_CHAR_RANGE->setEnabled(false);


}


void QNewMainWindow::setLangMenuActions() {
    // initial the language actions mapping table
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_ADA, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_ASP, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_ASSEMBLY, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_AUTO_IT, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_BATCH, createObject<QsciLexer, QsciLexerBatch> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_ANSI_C, createObject<QsciLexer, QsciLexerCPP> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_C_SHARP, createObject<QsciLexer, QsciLexerCSharp> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_CPP, createObject<QsciLexer, QsciLexerCPP> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_CAML, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_CMAKE, createObject<QsciLexer, QsciLexerCMake> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_COBOL, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_CSS, createObject<QsciLexer, QsciLexerCSS> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_ANSI_D, createObject<QsciLexer, QsciLexerD> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_DIFF, createObject<QsciLexer, QsciLexerDiff> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_FLASH, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_FORTRAN, createObject<QsciLexer, QsciLexerFortran> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_GUI4CLI, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_HASKELL, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_HTML, createObject<QsciLexer, QsciLexerHTML> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_INNO, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_JAVA, createObject<QsciLexer, QsciLexerJava> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_JAVASCRIPT, createObject<QsciLexer, QsciLexerJavaScript> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_JSP, createObject<QsciLexer, QsciLexerJavaScript> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_KIX, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_LISP, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_LUA, createObject<QsciLexer, QsciLexerLua> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_MAKEFILE, createObject<QsciLexer, QsciLexerMakefile> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_MATLAB, createObject<QsciLexer, QsciLexerMatlab> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_MS_INI, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_MSDOS, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_NORMAL_TEXT, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_NSIS, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_OBJECTIVE_C, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_PASCAL, createObject<QsciLexer, QsciLexerPascal> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_PERL, createObject<QsciLexer, QsciLexerPerl> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_PHP, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_POSTSCRIPT, createObject<QsciLexer, QsciLexerPostScript> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_POWERSHELL, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_PROPS, createObject<QsciLexer, QsciLexerProperties> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_PYTHON, createObject<QsciLexer, QsciLexerPython> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_ANSI_R, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_RESOURCE_FILE, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_RUBY, createObject<QsciLexer, QsciLexerRuby> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_BASH, createObject<QsciLexer, QsciLexerBash> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_SCHEME, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_SMALLTALK, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_SQL, createObject<QsciLexer, QsciLexerSQL> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_TCL, createObject<QsciLexer, QsciLexerTCL> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_TEX, createObject<QsciLexer, QsciLexerTeX> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_VB, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_VHDL, createObject<QsciLexer, QsciLexerVHDL> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_VERILOG, createObject<QsciLexer, QsciLexerVerilog> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_XML, createObject<QsciLexer, QsciLexerXML> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_YAML, createObject<QsciLexer, QsciLexerYAML> ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( ui->actionLANG_USER, NULL ));
    m_vtMenuLangActions.push_back(SActionMap<QsciLexer>( NULL, NULL ));

    // hide the unsupport language action
    for (int i=0; m_vtMenuLangActions[i].ptrAction; ++i) {
        if (m_vtMenuLangActions[i].ptrAction == ui->actionLANG_NORMAL_TEXT)
            continue;
        if (!m_vtMenuLangActions[i].fnFunc) {
            m_vtMenuLangActions[i].ptrAction->setEnabled(false);
        }
    }

    // hide the empty language sub menu
    _DEBUG_MSG("LANG_A menu conunt: %d", ui->menuLANG_A->actions().count());

    // Language menu actions
    m_langActionsGroup.addAction(ui->actionLANG_ADA);
    m_langActionsGroup.addAction(ui->actionLANG_ASP);
    m_langActionsGroup.addAction(ui->actionLANG_ASSEMBLY);
    m_langActionsGroup.addAction(ui->actionLANG_AUTO_IT);
    m_langActionsGroup.addAction(ui->actionLANG_BATCH);
    m_langActionsGroup.addAction(ui->actionLANG_ANSI_C);
    m_langActionsGroup.addAction(ui->actionLANG_C_SHARP);
    m_langActionsGroup.addAction(ui->actionLANG_CPP);
    m_langActionsGroup.addAction(ui->actionLANG_CAML);
    m_langActionsGroup.addAction(ui->actionLANG_CMAKE);
    m_langActionsGroup.addAction(ui->actionLANG_COBOL);
    m_langActionsGroup.addAction(ui->actionLANG_CSS);
    m_langActionsGroup.addAction(ui->actionLANG_ANSI_D);
    m_langActionsGroup.addAction(ui->actionLANG_DIFF);
    m_langActionsGroup.addAction(ui->actionLANG_FLASH);
    m_langActionsGroup.addAction(ui->actionLANG_FORTRAN);
    m_langActionsGroup.addAction(ui->actionLANG_GUI4CLI);
    m_langActionsGroup.addAction(ui->actionLANG_HASKELL);
    m_langActionsGroup.addAction(ui->actionLANG_HTML);
    m_langActionsGroup.addAction(ui->actionLANG_INNO);
    m_langActionsGroup.addAction(ui->actionLANG_JAVA);
    m_langActionsGroup.addAction(ui->actionLANG_JAVASCRIPT);
    m_langActionsGroup.addAction(ui->actionLANG_JSP);
    m_langActionsGroup.addAction(ui->actionLANG_KIX);
    m_langActionsGroup.addAction(ui->actionLANG_LISP);
    m_langActionsGroup.addAction(ui->actionLANG_LUA);
    m_langActionsGroup.addAction(ui->actionLANG_MAKEFILE);
    m_langActionsGroup.addAction(ui->actionLANG_MATLAB);
    m_langActionsGroup.addAction(ui->actionLANG_MS_INI);
    m_langActionsGroup.addAction(ui->actionLANG_MSDOS);
    m_langActionsGroup.addAction(ui->actionLANG_NORMAL_TEXT);
    m_langActionsGroup.addAction(ui->actionLANG_NSIS);
    m_langActionsGroup.addAction(ui->actionLANG_OBJECTIVE_C);
    m_langActionsGroup.addAction(ui->actionLANG_PASCAL);
    m_langActionsGroup.addAction(ui->actionLANG_PERL);
    m_langActionsGroup.addAction(ui->actionLANG_PHP);
    m_langActionsGroup.addAction(ui->actionLANG_POSTSCRIPT);
    m_langActionsGroup.addAction(ui->actionLANG_POWERSHELL);
    m_langActionsGroup.addAction(ui->actionLANG_PROPS);
    m_langActionsGroup.addAction(ui->actionLANG_PYTHON);
    m_langActionsGroup.addAction(ui->actionLANG_ANSI_R);
    m_langActionsGroup.addAction(ui->actionLANG_RESOURCE_FILE);
    m_langActionsGroup.addAction(ui->actionLANG_RUBY);
    m_langActionsGroup.addAction(ui->actionLANG_BASH);
    m_langActionsGroup.addAction(ui->actionLANG_SCHEME);
    m_langActionsGroup.addAction(ui->actionLANG_SMALLTALK);
    m_langActionsGroup.addAction(ui->actionLANG_SQL);
    m_langActionsGroup.addAction(ui->actionLANG_TCL);
    m_langActionsGroup.addAction(ui->actionLANG_TEX);
    m_langActionsGroup.addAction(ui->actionLANG_VB);
    m_langActionsGroup.addAction(ui->actionLANG_VHDL);
    m_langActionsGroup.addAction(ui->actionLANG_VERILOG);
    m_langActionsGroup.addAction(ui->actionLANG_XML);
    m_langActionsGroup.addAction(ui->actionLANG_YAML);
    m_langActionsGroup.addAction(ui->actionLANG_USER);

    connect(ui->actionLANG_ADA, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_ASP, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_ASSEMBLY, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_AUTO_IT, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_BATCH, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_ANSI_C, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_C_SHARP, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_CPP, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_CAML, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_CMAKE, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_COBOL, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_CSS, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_ANSI_D, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_DIFF, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_FLASH, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_FORTRAN, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_GUI4CLI, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_HASKELL, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_HTML, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_INNO, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_JAVA, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_JAVASCRIPT, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_JSP, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_KIX, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_LISP, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_LUA, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_MAKEFILE, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_MATLAB, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_MS_INI, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_MSDOS, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_NORMAL_TEXT, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_NSIS, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_OBJECTIVE_C, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_PASCAL, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_PERL, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_PHP, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_POSTSCRIPT, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_POWERSHELL, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_PROPS, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_PYTHON, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_ANSI_R, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_RESOURCE_FILE, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_RUBY, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_BASH, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_SCHEME, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_SMALLTALK, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_SQL, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_TCL, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_TEX, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_VB, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_VHDL, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_VERILOG, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_XML, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_YAML, SIGNAL(triggered()), this, SLOT(actionLang()));
    connect(ui->actionLANG_USER, SIGNAL(triggered()), this, SLOT(actionLang()));
}

void QNewMainWindow::setViewMenuActions() {
    ui->actionVIEW_ALWAYS_ON_TOP->setEnabled(false);
    ui->actionVIEW_FULL_SCREEN->setEnabled(false);
    ui->actionVIEW_POST_IT->setEnabled(false);
    ui->actionVIEW_SS_TAB_SPACE->setEnabled(false);
    ui->actionVIEW_SS_EOL->setEnabled(false);
    ui->actionVIEW_SS_ALL_CHARS->setEnabled(false);
    ui->actionVIEW_SS_INDENT_GUIDE->setEnabled(false);
    ui->actionVIEW_SS_WARP_SYMBOL->setEnabled(false);
    ui->actionVIEW_ZOOM_IN->setEnabled(false);
    ui->actionVIEW_ZOOM_OUT->setEnabled(false);
    ui->actionVIEW_ZOOM_DEFAULT->setEnabled(false);
    ui->actionVIEW_MOVE_OTHER_VIEW->setEnabled(false);
    ui->actionVIEW_CLONE_OTHER_VIEW->setEnabled(false);
    ui->actionVIEW_MOVE_NEW_INSTANCE->setEnabled(false);
    ui->actionVIEW_OPEN_NEW_INSTANCE->setEnabled(false);
    ui->actionVIEW_WARP->setEnabled(false);
    ui->actionVIEW_SWITCH_OTHER_VIEW->setEnabled(false);
    ui->actionVIEW_USER_DIALOG->setEnabled(false);
    ui->actionVIEW_FOLD_ALL->setEnabled(false);
    ui->actionVIEW_UNFOLD_ALL->setEnabled(false);
    ui->actionVIEW_FOLD_CURRENT->setEnabled(false);
    ui->actionVIEW_UNFOLD_CURRENT->setEnabled(false);
    ui->actionVIEW_COLLAPSE_1->setEnabled(false);
    ui->actionVIEW_COLLAPSE_2->setEnabled(false);
    ui->actionVIEW_COLLAPSE_3->setEnabled(false);
    ui->actionVIEW_COLLAPSE_4->setEnabled(false);
    ui->actionVIEW_COLLAPSE_5->setEnabled(false);
    ui->actionVIEW_COLLAPSE_6->setEnabled(false);
    ui->actionVIEW_COLLAPSE_7->setEnabled(false);
    ui->actionVIEW_COLLAPSE_8->setEnabled(false);
    ui->actionVIEW_UNCOLLAPSE_1->setEnabled(false);
    ui->actionVIEW_UNCOLLAPSE_2->setEnabled(false);
    ui->actionVIEW_UNCOLLAPSE_3->setEnabled(false);
    ui->actionVIEW_UNCOLLAPSE_4->setEnabled(false);
    ui->actionVIEW_UNCOLLAPSE_5->setEnabled(false);
    ui->actionVIEW_UNCOLLAPSE_6->setEnabled(false);
    ui->actionVIEW_UNCOLLAPSE_7->setEnabled(false);
    ui->actionVIEW_UNCOLLAPSE_8->setEnabled(false);
    ui->actionVIEW_SUMMARY->setEnabled(false);
    ui->actionVIEW_PROJECT_1->setEnabled(false);
    ui->actionVIEW_PROJECT_2->setEnabled(false);
    ui->actionVIEW_PROJECT_3->setEnabled(false);
    ui->actionVIEW_SYNC_VERT_SCROLL->setEnabled(false);
    ui->actionVIEW_SNYC_HORI_SCROLL->setEnabled(false);
    ui->actionVIEW_TEXT_RTL->setEnabled(false);
    ui->actionVIEW_TEXT_LTR->setEnabled(false);
}

void QNewMainWindow::setEncodingActions() {
    m_encodingActionsGroup.addAction(ui->actionENCODING_ANSI);
    m_encodingActionsGroup.addAction(ui->actionENCODING_UTF8_NO_BOM);
    m_encodingActionsGroup.addAction(ui->actionENCODING_UTF8);
    m_encodingActionsGroup.addAction(ui->actionENCODING_UCS2_BE);
    m_encodingActionsGroup.addAction(ui->actionENCODING_UCS2_LE);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_6);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM720);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1256);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_4);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_13);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM775);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1257);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_14);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_5);
    m_encodingActionsGroup.addAction(ui->actionENCODING_KOI8_R);
    m_encodingActionsGroup.addAction(ui->actionENCODING_KOI8_U);
    m_encodingActionsGroup.addAction(ui->actionENCODING_MACINTOSH);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_855);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_856);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1251);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_852);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1250);
    m_encodingActionsGroup.addAction(ui->actionENCODING_BIG5);
    m_encodingActionsGroup.addAction(ui->actionENCODING_GB2312);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_2);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_7);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_737);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_869);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1253);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_8);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_862);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1255);
    m_encodingActionsGroup.addAction(ui->actionENCODING_SHIFT_JIS);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_949);
    m_encodingActionsGroup.addAction(ui->actionENCODING_EUC_KR);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_861);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_865);
    m_encodingActionsGroup.addAction(ui->actionENCODING_TIS_620);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_3);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_9);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_857);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1254);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_1);
    m_encodingActionsGroup.addAction(ui->actionENCODING_ISO8859_15);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_850);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_858);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_860);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_863);
    m_encodingActionsGroup.addAction(ui->actionENCODING_OEM_US);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1252);
    m_encodingActionsGroup.addAction(ui->actionENCODING_WINDOW_1258);

    m_mapEncodings[ui->actionENCODING_ANSI]=            "System";
    m_mapEncodings[ui->actionENCODING_UTF8_NO_BOM]=	"UTF-8";
    m_mapEncodings[ui->actionENCODING_UTF8]=            "UTF-8";
    m_mapEncodings[ui->actionENCODING_UCS2_BE]=     	"UTF-16BE";
    m_mapEncodings[ui->actionENCODING_UCS2_LE]= 	"UTF-16LE";
    m_mapEncodings[ui->actionENCODING_ISO8859_6]=	"ISO-8859-6";
    m_mapEncodings[ui->actionENCODING_OEM720]=          "arabic";
    m_mapEncodings[ui->actionENCODING_WINDOW_1256]=	"Windows-1256";
    m_mapEncodings[ui->actionENCODING_ISO8859_4]=	"ISO-8859-4";
    m_mapEncodings[ui->actionENCODING_ISO8859_13]=	"ISO-8859-13";
    m_mapEncodings[ui->actionENCODING_OEM775]=          "latin4";
    m_mapEncodings[ui->actionENCODING_WINDOW_1257]=	"Windows-1257";
    m_mapEncodings[ui->actionENCODING_ISO8859_14]=	"ISO-8859-14";
    m_mapEncodings[ui->actionENCODING_ISO8859_5]=	"ISO-8859-5";
    m_mapEncodings[ui->actionENCODING_KOI8_R]=          "KOI8-R";
    m_mapEncodings[ui->actionENCODING_KOI8_U]=      	"KOI8-U";
    m_mapEncodings[ui->actionENCODING_MACINTOSH]=	"macintosh";
    m_mapEncodings[ui->actionENCODING_OEM_855]=         "cyrillic";
    m_mapEncodings[ui->actionENCODING_OEM_856]= 	"hebrew";
    m_mapEncodings[ui->actionENCODING_WINDOW_1251]=	"Windows-1251";
    m_mapEncodings[ui->actionENCODING_OEM_852]=         "latin2-1250";
    m_mapEncodings[ui->actionENCODING_WINDOW_1250]=	"Windows-1250";
    m_mapEncodings[ui->actionENCODING_BIG5]=            "Big5";
    m_mapEncodings[ui->actionENCODING_GB2312]=      	"GB18030-0";
    m_mapEncodings[ui->actionENCODING_ISO8859_2]=	"ISO-8859-2";
    m_mapEncodings[ui->actionENCODING_ISO8859_7]=	"ISO-8859-7";
    m_mapEncodings[ui->actionENCODING_OEM_737]=         "greek";
    m_mapEncodings[ui->actionENCODING_OEM_869]=     	"greek";
    m_mapEncodings[ui->actionENCODING_WINDOW_1253]=	"Windows-1253";
    m_mapEncodings[ui->actionENCODING_ISO8859_8]=	"ISO 8859-8";
    m_mapEncodings[ui->actionENCODING_OEM_862]=         "hebrew";
    m_mapEncodings[ui->actionENCODING_WINDOW_1255]=	"Windows-1255";
    m_mapEncodings[ui->actionENCODING_SHIFT_JIS]=	"Shift-JIS";
    m_mapEncodings[ui->actionENCODING_WINDOW_949]=	"CP949";
    m_mapEncodings[ui->actionENCODING_EUC_KR]=          "EUC-KR";
    m_mapEncodings[ui->actionENCODING_OEM_861]=         "";
    m_mapEncodings[ui->actionENCODING_OEM_865]=     	"";
    m_mapEncodings[ui->actionENCODING_TIS_620]=         "TIS-620";
    m_mapEncodings[ui->actionENCODING_ISO8859_3]=	"ISO-8859-3";
    m_mapEncodings[ui->actionENCODING_ISO8859_9]=	"ISO-8859-9";
    m_mapEncodings[ui->actionENCODING_OEM_857]=     	"";
    m_mapEncodings[ui->actionENCODING_WINDOW_1254]=	"Windows-1254";
    m_mapEncodings[ui->actionENCODING_ISO8859_1]=	"ISO-8859-1";
    m_mapEncodings[ui->actionENCODING_ISO8859_15]=	"ISO-8859-15";
    m_mapEncodings[ui->actionENCODING_OEM_850]=         "latin1";
    m_mapEncodings[ui->actionENCODING_OEM_858]=     	"";
    m_mapEncodings[ui->actionENCODING_OEM_860]=         "";
    m_mapEncodings[ui->actionENCODING_OEM_863]=         "";
    m_mapEncodings[ui->actionENCODING_OEM_US]=          "System";
    m_mapEncodings[ui->actionENCODING_WINDOW_1252]=	"Windows-1252";
    m_mapEncodings[ui->actionENCODING_WINDOW_1258]=	"Windows-1258";

    connect(ui->actionENCODING_ANSI, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_UTF8_NO_BOM, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_UTF8, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_UCS2_BE, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_UCS2_LE, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_6, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM720, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1256, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_4, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_13, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM775, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1257, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_14, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_5, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_KOI8_R, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_KOI8_U, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_MACINTOSH, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_855, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_856, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1251, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_852, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1250, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_BIG5, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_GB2312, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_2, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_7, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_737, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_869, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1253, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_8, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_862, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1255, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_SHIFT_JIS, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_949, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_EUC_KR, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_861, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_865, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_TIS_620, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_3, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_9, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_857, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1254, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_1, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_ISO8859_15, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_850, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_858, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_860, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_863, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_OEM_US, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1252, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_WINDOW_1258, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_CONV_ANSI, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_CONV_UTF8_NO_BOM, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_CONV_UTF8, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_CONV_UCS2_BE, SIGNAL(triggered()), this, SLOT(actionEncoding()));
    connect(ui->actionENCODING_CONV_UCS2_LE, SIGNAL(triggered()), this, SLOT(actionEncoding()));

#if 0
    ui->actionENCODING_ANSI->setEnabled(false);
    ui->actionENCODING_UTF8_NO_BOM->setEnabled(false);
    ui->actionENCODING_UTF8->setEnabled(false);
    ui->actionENCODING_UCS2_BE->setEnabled(false);
    ui->actionENCODING_UCS2_LE->setEnabled(false);
    ui->actionENCODING_ISO8859_6->setEnabled(false);
    ui->actionENCODING_OEM720->setEnabled(false);
    ui->actionENCODING_WINDOW_1256->setEnabled(false);
    ui->actionENCODING_ISO8859_4->setEnabled(false);
    ui->actionENCODING_ISO8859_13->setEnabled(false);
    ui->actionENCODING_OEM775->setEnabled(false);
    ui->actionENCODING_WINDOW_1257->setEnabled(false);
    ui->actionENCODING_ISO8859_14->setEnabled(false);
    ui->actionENCODING_ISO8859_5->setEnabled(false);
    ui->actionENCODING_KOI8_R->setEnabled(false);
    ui->actionENCODING_KOI8_U->setEnabled(false);
    ui->actionENCODING_MACINTOSH->setEnabled(false);
    ui->actionENCODING_OEM_855->setEnabled(false);
    ui->actionENCODING_OEM_856->setEnabled(false);
    ui->actionENCODING_WINDOW_1251->setEnabled(false);
    ui->actionENCODING_OEM_852->setEnabled(false);
    ui->actionENCODING_WINDOW_1250->setEnabled(false);
    //ui->actionENCODING_BIG5->setEnabled(false);
    //ui->actionENCODING_GB2312->setEnabled(false);
    ui->actionENCODING_ISO8859_2->setEnabled(false);
    ui->actionENCODING_ISO8859_7->setEnabled(false);
    ui->actionENCODING_OEM_737->setEnabled(false);
    ui->actionENCODING_OEM_869->setEnabled(false);
    ui->actionENCODING_WINDOW_1253->setEnabled(false);
    ui->actionENCODING_ISO8859_8->setEnabled(false);
    ui->actionENCODING_OEM_862->setEnabled(false);
    ui->actionENCODING_WINDOW_1255->setEnabled(false);
    ui->actionENCODING_SHIFT_JIS->setEnabled(false);
    ui->actionENCODING_WINDOW_949->setEnabled(false);
    ui->actionENCODING_EUC_KR->setEnabled(false);
    ui->actionENCODING_OEM_861->setEnabled(false);
    ui->actionENCODING_OEM_865->setEnabled(false);
    ui->actionENCODING_TIS_620->setEnabled(false);
    ui->actionENCODING_ISO8859_3->setEnabled(false);
    ui->actionENCODING_ISO8859_9->setEnabled(false);
    ui->actionENCODING_OEM_857->setEnabled(false);
    ui->actionENCODING_WINDOW_1254->setEnabled(false);
    ui->actionENCODING_ISO8859_1->setEnabled(false);
    ui->actionENCODING_ISO8859_15->setEnabled(false);
    ui->actionENCODING_OEM_850->setEnabled(false);
    ui->actionENCODING_OEM_858->setEnabled(false);
    ui->actionENCODING_OEM_860->setEnabled(false);
    ui->actionENCODING_OEM_863->setEnabled(false);
    ui->actionENCODING_OEM_US->setEnabled(false);
    ui->actionENCODING_WINDOW_1252->setEnabled(false);
    ui->actionENCODING_WINDOW_1258->setEnabled(false);
    ui->actionENCODING_CONV_ANSI->setEnabled(false);
    ui->actionENCODING_CONV_UTF8_NO_BOM->setEnabled(false);
    ui->actionENCODING_CONV_UTF8->setEnabled(false);
    ui->actionENCODING_CONV_UCS2_BE->setEnabled(false);
    ui->actionENCODING_CONV_UCS2_LE->setEnabled(false);
#endif
}

void QNewMainWindow::setSettingsActions() {
    ui->actionSETTINGS_PERFERENCES->setEnabled(false);
    ui->actionSETTINGS_LANG_STYLE->setEnabled(false);
    ui->actionSETTINGS_SHORTCUT_MAPPER->setEnabled(false);
    ui->actionSETTINGS_IMPORT_PLUGIN->setEnabled(false);
    ui->actionSETTINGS_IMPORT_STYLE_THEME->setEnabled(false);
    ui->actionSETTINGS_EDIT_CONTEXTMENU->setEnabled(false);
}

void QNewMainWindow::setMacroActions() {
    ui->actionMACRO_RECORD->setEnabled(false);
    ui->actionMACRO_STOP->setEnabled(false);
    ui->actionMACRO_PLAYBACK->setEnabled(false);
    ui->actionMACRO_SAVE->setEnabled(false);
    ui->actionMACRO_RUN_MULTI_MACRO->setEnabled(false);
}

void QNewMainWindow::setRunActions() {
    ui->actionRUN_RUN->setEnabled(false);
}

void QNewMainWindow::setHelpActions() {
    ui->actionHELP_HELP->setEnabled(false);
    ui->actionHELP_HOMEPAGE->setEnabled(false);
    ui->actionHELP_PROJECTPAGE->setEnabled(false);
    ui->actionHELP_ONLINE_DOC->setEnabled(false);
    ui->actionHELP_FORUM->setEnabled(false);
    ui->actionHELP_GET_PLUGINS->setEnabled(false);
    ui->actionHELP_UPDATE_QPAD->setEnabled(false);
    ui->actionHELP_ABOUT->setEnabled(false);
}

int QNewMainWindow::closeSubWinFile(QMdiSubWindow *ptrSubWin) {
    if (!ptrSubWin) return -1;

    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return -1;

    int nRet=-1;

    if (ptrEdit->isModified()) {
        nRet=QMessageBox::question(this, IDS_SAVE_FILE_TITLE,
                            IDS_WANT_TO_SAVE_FILE.arg(reinterpret_cast<QPadMdiSubWindow*>(ptrSubWin)->m_qstrFileName),
                            QMessageBox::No | QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::No);

        switch(nRet) {
            default:
            case QMessageBox::Cancel:
                return nRet;
            case QMessageBox::No:
                break;
            case QMessageBox::Ok:
            {
                QString qstr=reinterpret_cast<QPadMdiSubWindow*>(ptrSubWin)->m_qstrFileName;

                this->saveDoc(0 == QString::compare(qstr.left(strlen(_NEW_FILE_PREFIX)), _NEW_FILE_PREFIX) ?
                              "" : qstr,
                              reinterpret_cast<QPadMdiSubWindow*>(ptrSubWin));
            }    break;
        }

    }
    ptrSubWin->close();
    return nRet;
}

void QNewMainWindow::SetScintillaEditMenu(QMdiSubWindow *ptrSubWin) {

}

QPadMdiSubWindow* QNewMainWindow::findSubWinsFilename(QString qstr) {
    QList<QMdiSubWindow *> list=m_pMdiArea->subWindowList();
    for (QList<QMdiSubWindow*>::iterator pFind=list.begin(); pFind != list.end(); ++pFind) {
        if(reinterpret_cast<QPadMdiSubWindow*>(*pFind)->m_qstrFileName == qstr) {
            _DEBUG_MSG("find the same file in sub win: 0x%x", *pFind);
            return reinterpret_cast<QPadMdiSubWindow*>(*pFind);
        }
    }
    return NULL;
}

int QNewMainWindow::reloadFileWithCharset(char *charset) {
    int nRet=-1;
    if (NULL == charset)
        return nRet;

    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return nRet;

    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return nRet;

    QFile file(ptrSubWin->m_qstrFileName);
    if (!file.open(QFile::ReadOnly)) {
        _DEBUG_MSG("open file %s failed", ptrSubWin->m_qstrFileName.toAscii().data());
        return nRet;
    }
    QByteArray dbBuffer=file.readAll();
    file.close();

    QTextCodec *codec = QTextCodec::codecForName(charset);
    if (!codec) return nRet;

    QString qstrUCS2=codec->toUnicode(dbBuffer);
    dbBuffer.clear();

    ptrEdit->setUtf8(true);
    ptrEdit->setText(qstrUCS2);
    ptrSubWin->m_qstrCharset=charset;
    ptrEdit->setModified(false);
}

void QNewMainWindow::actionFileNew() {
    addDocPanel("");
}

void QNewMainWindow::actionFileOpen(){
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::AnyFile);
    QStringList fileList;
    if (dlg.exec()) {
        fileList=dlg.selectedFiles();
    }
    _DEBUG_MSG("selected file number: %d", fileList.count());
    if (0 == fileList.count())
        return;

    for (int i=0; i<fileList.count(); ++i) {
        QPadMdiSubWindow *ptrSubWin=this->findSubWinsFilename(fileList.at(i));
        if (ptrSubWin) {
            _DEBUG_MSG("the file: %s is already opened", fileList.at(i).toAscii().data());
            m_pMdiArea->setActiveSubWindow(ptrSubWin);
            continue;
        }

        addDocPanel(fileList.at(i));
        _DEBUG_MSG("mdi subwindow list: %d", m_pMdiArea->subWindowList().count());
    }
}

void QNewMainWindow::actionFileSave() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;

    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());

    // detect is the file new file?
    _DEBUG_MSG("filename: %s", ptrSubWin->m_qstrFileName.toAscii().data());
    if (0 == QString::compare(ptrSubWin->m_qstrFileName.left(strlen(_NEW_FILE_PREFIX)), _NEW_FILE_PREFIX)) {
        QString qstrFile=this->saveDoc("", ptrSubWin);
        if (qstrFile.isEmpty()) return;

        ptrSubWin->m_qstrFileName=qstrFile;
        QString filename=qstrFile.mid(qstrFile.lastIndexOf('/')+1);
        ptrSubWin->setWindowTitle(filename.append(" [*]"));

        ptrEdit->setModified(false);

    }
    else {
        if (ptrEdit->isModified()) {
            this->saveDoc(ptrSubWin->m_qstrFileName, ptrSubWin);
            ptrEdit->setModified(false);
        }
    }

}

void QNewMainWindow::actionFileSaveAs() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;

    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());

    QString qstrFile=this->saveDoc("", ptrSubWin);
    if (qstrFile.isEmpty()) return;

    ptrSubWin->m_qstrFileName=qstrFile;

    ptrEdit->setModified(false);

    QString filename=qstrFile.mid(qstrFile.lastIndexOf('/')+1);
    ptrSubWin->setWindowTitle(filename.append(" [*]"));


}

void QNewMainWindow::actionFileSaveAsCopy() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;

    this->saveDoc("", ptrSubWin);
}

void QNewMainWindow::actionFileSaveAll() {
    QList<QMdiSubWindow*> list=m_pMdiArea->subWindowList();
    QList<QMdiSubWindow*>::iterator pFind;
    for (pFind=list.begin(); pFind != list.end(); ++pFind) {
        // detect is the file new file?
        QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(*pFind);
        QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());

        _DEBUG_MSG("filename: %s", ptrSubWin->m_qstrFileName.toAscii().data());
        if (0 == QString::compare(ptrSubWin->m_qstrFileName.left(strlen(_NEW_FILE_PREFIX)), _NEW_FILE_PREFIX)) {
            QString qstrFile=this->saveDoc("", ptrSubWin);
            if (qstrFile.isEmpty()) continue;

            ptrSubWin->m_qstrFileName=qstrFile;
            QString filename=qstrFile.mid(qstrFile.lastIndexOf('/')+1);
            ptrSubWin->setWindowTitle(filename.append(" [*]"));

            ptrEdit->setModified(false);

        }
        else {
            if (ptrEdit->isModified()) {
                this->saveDoc(ptrSubWin->m_qstrFileName, ptrSubWin);
                ptrEdit->setModified(false);
            }
        }
        ptrSubWin->setWindowModified(ptrEdit->isModified());

        setUiMenuItem(ptrSubWin);
    }
}


void QNewMainWindow::actionFileReload() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;

    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    QFile file(ptrSubWin->m_qstrFileName);
    if(!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, IDS_FILE_OPEN_FAILED_TITLE,
                             IDS_FILE_OPEN_FAILED_CONTENT.arg(ptrSubWin->m_qstrFileName.arg(file.errorString())));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ptrEdit->read(&file);
    file.close();
    QApplication::restoreOverrideCursor();
    ptrEdit->setModified(false);
}

void QNewMainWindow::actionFileClose() {
    QMdiSubWindow *ptrSubWin=this->getMdiActiveWindow();
    closeSubWinFile(ptrSubWin);
}

void QNewMainWindow::actionFileExit() {
    actionFileCloseAll();
    QApplication::quit();
}

void QNewMainWindow::actionFileCloseAll() {
    QList<QMdiSubWindow*> list=m_pMdiArea->subWindowList();
    for (QList<QMdiSubWindow*>::iterator pFind=list.begin(); pFind != list.end(); ++pFind) {
        if (QMessageBox::Cancel == closeSubWinFile(*pFind))
            break;
    }
}

void QNewMainWindow::actionFileCloseAllExceptCurrent() {
    QList<QMdiSubWindow*> list=m_pMdiArea->subWindowList();
    for (QList<QMdiSubWindow*>::iterator pFind=list.begin(); pFind != list.end(); ++pFind) {
        if (*pFind == this->getMdiActiveWindow())
            continue;
        closeSubWinFile(*pFind);
    }
}

void QNewMainWindow::actionFilePrint() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    QsciPrinter printer;
    QPrintDialog dlg(&printer);

    if (QDialog::Accepted == dlg.exec()) {
        printer.printRange(ptrEdit);
    }
}

void QNewMainWindow::actionFilePrintNow() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    QsciPrinter printer;
    printer.printRange(ptrEdit);
}

void QNewMainWindow::actionFileRename() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;

    if (0 == QString::compare(ptrSubWin->m_qstrFileName.left(strlen(_NEW_FILE_PREFIX)), _NEW_FILE_PREFIX))
        return;

    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    if (dlg.exec()) {
        QStringList list=dlg.selectedFiles();
        QString qstrFile=*list.begin();

        QFile::rename(ptrSubWin->m_qstrFileName, qstrFile);
        ptrSubWin->m_qstrFileName=qstrFile;
        QString filename=qstrFile.mid(qstrFile.lastIndexOf('/')+1);
        ptrSubWin->setWindowTitle(filename.append(" [*]"));
    }
}

void QNewMainWindow::actionEditUndo() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    ptrEdit->undo();
}

void QNewMainWindow::actionEditRedo() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    ptrEdit->redo();
}

void QNewMainWindow::actionEditCut() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    ptrEdit->cut();
}

void QNewMainWindow::actionEditCopy() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    ptrEdit->copy();
}

void QNewMainWindow::actionEditPaste() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    ptrEdit->paste();
}

void QNewMainWindow::actionEditDel() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    QKeyEvent key_down(QKeyEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier, NULL, false, 1);
    QKeyEvent key_up(QKeyEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier, NULL, false, 1);
    QApplication::sendEvent(ptrEdit, &key_down);
    QApplication::sendEvent(ptrEdit, &key_up);
}

void QNewMainWindow::actionEditSelectAll() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    ptrEdit->selectAll();
}

void QNewMainWindow::actionLang() {
    int i=0;
    while (m_vtMenuLangActions[i].ptrAction) {
        if(m_vtMenuLangActions[i].ptrAction->isChecked()) {
            QMdiSubWindow *ptrSubWin=this->getMdiActiveWindow();
            if (!ptrSubWin) {
                _DEBUG_MSG("null sub window pointer");
                break;
            }

            QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>
                    (ptrSubWin->widget());//(ptrSubWin->userData(EUSERDATA_SCINTILLA_TEXT_EDITOR));
            if (!ptrEdit) {
                _DEBUG_MSG("null text editor pointer");
                break;
            }

            QsciLexer *p=ptrEdit->lexer();
            ptrEdit->setLexer(0);
            _DEL_MEM(p);
            ptrEdit->setLexer(NULL == m_vtMenuLangActions[i].fnFunc ? 0 : m_vtMenuLangActions[i].fnFunc());
            break;
        }
        ++i;
    }
}

void QNewMainWindow::actionSearchBookmark() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    int nLine=-1, nIndex=-1;
    ptrEdit->getCursorPosition(&nLine, &nIndex);
    ptrEdit->SendScintilla(ptrEdit->SendScintilla(SCI_MARKERGET, nLine) ? SCI_MARKERDELETE : SCI_MARKERADD, nLine, _BOOKMARK_NUM);
}

void QNewMainWindow::actionSearchBookmarkNext() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    int nLine=-1, nIndex=-1;
    ptrEdit->getCursorPosition(&nLine, &nIndex);
    int nNextLine=ptrEdit->SendScintilla(SCI_MARKERNEXT, nLine+1, 1 << _BOOKMARK_NUM);
    _DEBUG_MSG("line: %d, next line: %d", nLine, nNextLine);
    if (-1 == nNextLine)
        nNextLine=ptrEdit->SendScintilla(SCI_MARKERNEXT, 0, 1 << _BOOKMARK_NUM);
    if (-1 == nNextLine)
        return;

    ptrEdit->SendScintilla(SCI_GOTOLINE, nNextLine);
}

void QNewMainWindow::actionSearchBookmarkPrev() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    int nLine=-1, nIndex=-1;
    ptrEdit->getCursorPosition(&nLine, &nIndex);
    int nPrevLine=ptrEdit->SendScintilla(SCI_MARKERPREVIOUS, nLine-1, 1 << _BOOKMARK_NUM);
    _DEBUG_MSG("line: %d, prev line: %d", nLine, nPrevLine);
    if (-1 == nPrevLine)
        nPrevLine=ptrEdit->SendScintilla(SCI_MARKERPREVIOUS, ptrEdit->lines(), 1 << _BOOKMARK_NUM);
    if (-1 == nPrevLine)
        return;

    ptrEdit->SendScintilla(SCI_GOTOLINE, nPrevLine);
}

void QNewMainWindow::actionSearchGotoLine() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    int nMaxLine=ptrEdit->lines();
    int nLine=-1, nIndex=-1;
    ptrEdit->getCursorPosition(&nLine, &nIndex);
    int nPos=ptrEdit->SendScintilla(SCI_GETCURRENTPOS);
    _DEBUG_MSG("line:%d, index:%d, pos: %d", nLine, nIndex, nPos);

    QPadGotoLineDialog dlg;
    dlg.m_nMaxLine=nMaxLine;
    dlg.m_nCurrentLine=nLine;
    dlg.m_nCurrentOffset=nPos;
    dlg.m_nMaxOffset=ptrEdit->length();
    if (QDialog::Accepted == dlg.exec()) {
        if (QPadGotoLineDialog::ETYPE_LINE == dlg.m_nType) {
            ptrEdit->SendScintilla(SCI_GOTOLINE, 0 < dlg.m_nCurrentLine ? dlg.m_nCurrentLine-1: nLine);
        }
        else {
            ptrEdit->SendScintilla(SCI_GOTOPOS, 0 <= dlg.m_nCurrentOffset ? dlg.m_nCurrentOffset: nPos);
        }
    }
}

void QNewMainWindow::actionSearchFind() {
    if (!m_pFindDlg) {
        m_pFindDlg=new QPadFindReplaceDialog(this);
        m_pFindDlg->m_nInitIndex=QPadFindReplaceDialog::EFUNC_FIND;

        connect(m_pFindDlg, SIGNAL(sigOnCloseDlg()), this, SLOT(slotOnCloseFindDialog()));
        connect(m_pFindDlg, SIGNAL(sigFindFindNext(QPadFindReplaceDialog::SValue,QString)),
                this, SLOT(slotOnFindFindNext(QPadFindReplaceDialog::SValue,QString)));
    }

    m_pFindDlg->show();
    m_pFindDlg->raise();
    m_pFindDlg->activateWindow();
}

void QNewMainWindow::actionEncoding() {
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    QMap<QAction*, QString>::iterator pFind;
    for (pFind=m_mapEncodings.begin(); pFind != m_mapEncodings.end(); ++pFind) {
        if (pFind.key()->isChecked()) {
            reloadFileWithCharset(pFind.value().toAscii().data());
            return;
        }
    }
}

bool QNewMainWindow::addDocPanel(QString str) {
    bool bRet=false;
    QPadMdiSubWindow *pSubWin=new QPadMdiSubWindow(m_pMdiArea->viewport());
    QsciScintilla *pEdit=new QsciScintilla;
    pSubWin->setAttribute(Qt::WA_DeleteOnClose);
    pSubWin->setWidget(pEdit);
    m_pMdiArea->addSubWindow(pSubWin);
    pSubWin->showMaximized();

    if (str.isEmpty()) {
        str.sprintf("%s%d", _NEW_FILE_PREFIX, m_nNewDocNum++);
        pSubWin->m_qstrFileName=str;

        QsciLexerCPP *lexer=new QsciLexerCPP(pEdit);
        pEdit->setLexer(lexer);

        pSubWin->setWindowTitle(str.append(" [*]"));
//        manager.pMdiSubWidget->setUserData(
//            EUSERDATA_SCINTILLA_TEXT_EDITOR,
//            reinterpret_cast<QObjectUserData*>(manager.pTextEditor));
    }
    else {
        QFile *file=new QFile(str);
        if(!file->open(QFile::ReadOnly)) {
            QMessageBox::warning(this, IDS_FILE_OPEN_FAILED_TITLE,
                                 IDS_FILE_OPEN_FAILED_CONTENT.arg(str).arg(file->errorString()));
            _DEL_MEM(file);
            return bRet;
        }

        QApplication::setOverrideCursor(Qt::WaitCursor);
        pSubWin->m_qstrFileName=str;
        pEdit->read(file);
        reloadFileWithCharset("Big5");
        QApplication::restoreOverrideCursor();

//        manager.pMdiSubWidget->setUserData(
//            EUSERDATA_SCINTILLA_TEXT_EDITOR,
//            reinterpret_cast<QObjectUserData*>(manager.pTextEditor));

        QString filename=str.mid(str.lastIndexOf('/')+1);
        pSubWin->setWindowTitle(filename.append(" [*]"));

        file->close();
        _DEL_MEM(file);

    }
    m_pMdiArea->setActiveSubWindow(pSubWin);

    connect(pEdit, SIGNAL(modificationChanged(bool)), this, SLOT(slotDocWasModified()));
    connect(pSubWin, SIGNAL(sigCloseSubWindow(QMdiSubWindow*)), this, SLOT(slotOnCloseSubWindow(QMdiSubWindow*)));
    pEdit->setMarginWidth(1, QString("######"));
    pEdit->setMarginType(1, QsciScintilla::SymbolMarginDefaultBackgroundColor);
    pEdit->setMarginLineNumbers(1, true);
    pEdit->markerDefine(QsciScintilla::RightTriangle, _BOOKMARK_NUM);

    pEdit->setModified(false);

    bRet=true;
    return bRet;
}

QMdiSubWindow* QNewMainWindow::getMdiActiveWindow() {
    if (1 >= m_pMdiArea->subWindowList().count()) {
        _DEBUG_MSG("only 1 sub window");
        return *m_pMdiArea->subWindowList().begin();
    }
    else {
        _DEBUG_MSG("multiple sub window");
        return NULL == m_pMdiArea->activeSubWindow() ? m_pMdiArea->currentSubWindow() : m_pMdiArea->activeSubWindow();
    }
}

QString QNewMainWindow::saveDoc(QString qstrFile, QPadMdiSubWindow *ptrSubWin) {
    if (!ptrSubWin)
        return QString("");

    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());

    // is new file?
    if (qstrFile.isEmpty()) {
        QFileDialog dlg;
        dlg.setFileMode(QFileDialog::AnyFile);
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        if (dlg.exec()) {
            QStringList list=dlg.selectedFiles();
            qstrFile=*list.begin();
        }
    }
    _DEBUG_MSG("save filename: %s", qstrFile.toAscii().data());
    if (qstrFile.isEmpty()) return QString("");

    QFile file(qstrFile);
    if (!file.open(QFile::WriteOnly)) {
        _DEBUG_MSG("open be saved file failed");
        QMessageBox::warning(this, IDS_FILE_OPEN_FAILED_TITLE,
                             IDS_FILE_OPEN_FAILED_CONTENT.arg(qstrFile).arg(file.errorString()));
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ptrEdit->write(&file);
    QApplication::restoreOverrideCursor();

    file.close();

    return qstrFile;
}

void QNewMainWindow::setUiMenuItem(QMdiSubWindow *ptrSubWin) {
    if (!ptrSubWin) return;
    QPadMdiSubWindow *ptrSubQWin=reinterpret_cast<QPadMdiSubWindow*>(ptrSubWin);

    QsciScintilla* ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());//(ptrSubWin->userData(EUSERDATA_SCINTILLA_TEXT_EDITOR));
    if (!ptrEdit) return;

    _DEBUG_MSG("file name: %s", ptrSubQWin->m_qstrFileName.toAscii().data());

    // process the menu action -- File
    ui->actionFILE_SAVE->setEnabled(ptrEdit->isModified());

    if(0 == QString::compare(ptrSubQWin->m_qstrFileName.left(strlen(_NEW_FILE_PREFIX)), _NEW_FILE_PREFIX)) {
        ui->actionFILE_RELOAD->setEnabled(false);
        ui->actionFILE_RENAME->setEnabled(false);
    }
    else {
        ui->actionFILE_RELOAD->setEnabled(true);
        ui->actionFILE_RENAME->setEnabled(true);
    }

    ui->actionFILE_SAVEAS->setEnabled(!ptrEdit->text().isEmpty());
    ui->actionFILE_SAVE_COPY->setEnabled(!ptrEdit->text().isEmpty());


    ui->actionFILE_CLOSE->setEnabled(NULL != this->getMdiActiveWindow());
    ui->actionFILE_CLOSE_ALL->setEnabled(NULL != this->getMdiActiveWindow());

    // Edit
    ui->actionEDIT_UNDO->setEnabled(ptrEdit->isModified());
    ui->actionEDIT_REDO->setEnabled(ptrEdit->isRedoAvailable());

}

void QNewMainWindow::changeEvent(QEvent * event) {
    BaseMainWindow::changeEvent(event);
    switch(event->type()) {
        default:
            break;
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
    }
}

void QNewMainWindow::closeEvent(QCloseEvent *event) {
    if (m_pFindDlg) {
        m_pFindDlg->close();
    }
}

void QNewMainWindow::slotCreate() {
    _DEBUG_MSG("%s", __PRETTY_FUNCTION__);
    ui->setupUi(this);
    ui->retranslateUi(this);

    // setup the ui object's actions
    setMenuActions();
    setUnmenuActoins();

    this->resize(QApplication::desktop()->width()/5*4, QApplication::desktop()->height()/5*4);
    BaseMainWindow::slotCreate();
    // initial mdi area
    m_pMdiArea = new QMdiArea;
    m_pMdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pMdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(m_pMdiArea);

    // remove the system icon -- close, max, mini
    m_pMdiArea->setWindowFlags(Qt::CustomizeWindowHint);

    m_pMdiArea->setViewMode(QMdiArea::TabbedView);
    m_pMdiArea->setTabShape(QTabWidget::Triangular);

    connect(m_pMdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(slotOnChangedSubWindow(QMdiSubWindow*)));

    // initial scintilla text editor
    addDocPanel("");
    QMdiSubWindow *ptrSubWin=this->getMdiActiveWindow();
    if (!ptrSubWin) return;
    QsciScintilla* ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());//(ptrSubWin->userData(EUSERDATA_SCINTILLA_TEXT_EDITOR));
    if (!ptrEdit) return;
    slotDocWasModified();

    // Dock widget test
    m_pDockFindResult=new QPadDockFindResultWidget(this);

    addDockWidget(Qt::BottomDockWidgetArea, m_pDockFindResult);

    this->show();
}

void QNewMainWindow::slotAppCmd(QString qstr) {
    _DEBUG_MSG("%s", qstr.toAscii().data());
    this->showMinimized();
    this->activateWindow();
}

void QNewMainWindow::slotDocWasModified() {
    _DEBUG_MSG("is the document modified??");
    QMdiSubWindow *ptrSubWin=this->getMdiActiveWindow();
    _DEBUG_MSG("ptrSubWin: 0x%x", ptrSubWin);
    if (!ptrSubWin) return;
    QsciScintilla* ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());//(ptrSubWin->userData(EUSERDATA_SCINTILLA_TEXT_EDITOR));
    _DEBUG_MSG("ptrEdit: 0x%x", ptrEdit);
    if (!ptrEdit) return;
    _DEBUG_MSG("doucment modified: %d", ptrEdit->isModified());
    ptrSubWin->setWindowModified(ptrEdit->isModified());

    setUiMenuItem(ptrSubWin);

}

void QNewMainWindow::slotOnChangedSubWindow(QMdiSubWindow *ptrSubWin) {
    setUiMenuItem(ptrSubWin);
}

void QNewMainWindow::slotOnCloseSubWindow(QMdiSubWindow *ptrSubWin) {
    if (!ptrSubWin) return;
}

void QNewMainWindow::slotOnCloseFindDialog() {
    _DEL_MEM(m_pFindDlg);
}

void QNewMainWindow::slotOnFindFindNext(QPadFindReplaceDialog::SValue value, QString expression) {
    _DEBUG_MSG("+++");
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(this->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    int nLine=-1, nIndex=-1;
    ptrEdit->getCursorPosition(&nLine, &nIndex);
    _DEBUG_MSG("line: %d, index: %d\n", nLine, nIndex);

    if (value.nDirection == QPadFindReplaceDialog::EDIR_UP) {
        int nBLine, nELine, nBIndex, nEIndex;
        ptrEdit->getSelection(&nBLine, &nBIndex, &nELine, &nEIndex);
        nIndex= (nIndex - (nEIndex - nBIndex) < 0 ? 0 : nIndex - (nEIndex - nBIndex));
    }

    bool bResult=ptrEdit->findFirst(expression, value.nMode == QPadFindReplaceDialog::EMODE_REGEX,
                       (value.nFeature & QPadFindReplaceDialog::EFEATURE_CASE) == QPadFindReplaceDialog::EFEATURE_CASE,
                       (value.nFeature & QPadFindReplaceDialog::EFEATURE_WHOLE_WORD) == QPadFindReplaceDialog::EFEATURE_WHOLE_WORD,
                       (value.nFeature & QPadFindReplaceDialog::EFEATURE_WARP) == QPadFindReplaceDialog::EFEATURE_WARP,
                       value.nDirection == QPadFindReplaceDialog::EDIR_DOWN, nLine, nIndex, true, true);

    _DEBUG_MSG("find result: %d", bResult);
}
