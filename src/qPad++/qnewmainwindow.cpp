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

QNewMainWindow::QNewMainWindow(QWidget *parent) :
    BaseMainWindow(parent),
    ui(new Ui::QNewMainWindow),
    m_langActionsGroup(this)
{
    m_pMdiArea=NULL;
    m_nNewDocNum=1;
    m_vtMenuLangActions.clear();


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
}

void QNewMainWindow::setMenuActions() {
    setFileMenuActions();
    setEditMenuActions();
    setLangMenuActions();
    setSearchMenuActions();
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
            m_vtMenuLangActions[i].ptrAction->setVisible(false);
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

}

void QNewMainWindow::actionFilePrintNow() {

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
        pEdit->read(file);
        QApplication::restoreOverrideCursor();

//        manager.pMdiSubWidget->setUserData(
//            EUSERDATA_SCINTILLA_TEXT_EDITOR,
//            reinterpret_cast<QObjectUserData*>(manager.pTextEditor));
        pSubWin->m_qstrFileName=str;
        QString filename=str.mid(str.lastIndexOf('/')+1);
        pSubWin->setWindowTitle(filename.append(" [*]"));

        file->close();
        _DEL_MEM(file);

    }
    m_pMdiArea->setActiveSubWindow(pSubWin);

    connect(pEdit, SIGNAL(modificationChanged(bool)), this, SLOT(slotDocWasModified()));
    connect(pSubWin, SIGNAL(sigCloseSubWindow(QMdiSubWindow*)), this, SLOT(slotOnCloseSubWindow(QMdiSubWindow*)));
    pEdit->setMarginWidth(1, QString("####"));
    pEdit->setMarginType(1, QsciScintilla::SymbolMargin);
    pEdit->setMarginLineNumbers(1, true);

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
