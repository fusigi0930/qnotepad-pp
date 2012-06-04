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

QNewMainWindow::QNewMainWindow(QWidget *parent) :
    BaseMainWindow(parent),
    ui(new Ui::QNewMainWindow),
    m_langActionsGroup(this)
{
    m_pMdiArea=NULL;
    m_nNewDocNum=1;
}

QNewMainWindow::~QNewMainWindow()
{
    _DEL_MEM(ui);

    QMap<QString, STextManager>::iterator pFind;
    for (pFind=m_mapOpenedFiles.begin(); pFind != m_mapOpenedFiles.end(); ++pFind) {
        _DEBUG_MSG("delete memory: 0x%x", pFind.value().pTextEditor);
        m_pMdiArea->removeSubWindow(pFind.value().ptrMdiSubWidget);
        QsciLexer *p=pFind.value().pTextEditor->lexer();
        _DEL_MEM(p);
        pFind.value().pTextEditor->setLexer(0);
        pFind.value().pTextEditor->close();
        _DEL_MEM(pFind.value().pTextEditor);
        _DEL_MEM(pFind.value().pFile);
    }
    m_mapOpenedFiles.clear();
    m_pMdiArea->closeAllSubWindows();
    _DEL_MEM(m_pMdiArea);
}

void QNewMainWindow::setMenuActions() {
    // File menu actions
    ui->actionFILE_NEW->setShortcuts(QKeySequence::New);
    connect(ui->actionFILE_NEW, SIGNAL(triggered()), this, SLOT(actionFileNew()));

    ui->actionFILE_OPEN->setShortcuts(QKeySequence::Open);
    connect(ui->actionFILE_OPEN, SIGNAL(triggered()), this, SLOT(actionFileOpen()));

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
        QMap<QString, STextManager>::iterator pFind=m_mapOpenedFiles.find(fileList.at(i));
        if (pFind != m_mapOpenedFiles.end()) {
            _DEBUG_MSG("the file: %s is already opened", fileList.at(i).toAscii().data());
            m_pMdiArea->setActiveSubWindow(pFind.value().ptrMdiSubWidget);
            continue;
        }

        QFile *file=new QFile(fileList.at(i));
        if(!file->open(QFile::ReadWrite)) {
            QMessageBox::warning(this, IDS_FILE_OPEN_FAILED_TITLE,
                                 IDS_FILE_OPEN_FAILED_CONTENT.arg(fileList.at(i)).arg(file->errorString()));
            _DEL_MEM(file);
        }
        addDocPanel(fileList.at(i));
        _DEBUG_MSG("mdi subwindow list: %d", m_pMdiArea->subWindowList().count());
    }
}

void QNewMainWindow::actionLang() {
    SActionMap<QsciLexer> menuLangActionsMap[] = {
        { ui->actionLANG_ADA, NULL },
        { ui->actionLANG_ASP, NULL },
        { ui->actionLANG_ASSEMBLY, NULL },
        { ui->actionLANG_AUTO_IT, NULL },
        { ui->actionLANG_BATCH, createObject<QsciLexer, QsciLexerBatch> },
        { ui->actionLANG_ANSI_C, createObject<QsciLexer, QsciLexerCPP> },
        { ui->actionLANG_C_SHARP, createObject<QsciLexer, QsciLexerCSharp> },
        { ui->actionLANG_CPP, createObject<QsciLexer, QsciLexerCPP> },
        { ui->actionLANG_CAML, NULL },
        { ui->actionLANG_CMAKE, createObject<QsciLexer, QsciLexerCMake> },
        { ui->actionLANG_COBOL, NULL },
        { ui->actionLANG_CSS, createObject<QsciLexer, QsciLexerCSS> },
        { ui->actionLANG_ANSI_D, createObject<QsciLexer, QsciLexerD> },
        { ui->actionLANG_DIFF, createObject<QsciLexer, QsciLexerDiff> },
        { ui->actionLANG_FLASH, NULL },
        { ui->actionLANG_FORTRAN, createObject<QsciLexer, QsciLexerFortran> },
        { ui->actionLANG_GUI4CLI, NULL },
        { ui->actionLANG_HASKELL, NULL },
        { ui->actionLANG_HTML, createObject<QsciLexer, QsciLexerHTML> },
        { ui->actionLANG_INNO, NULL },
        { ui->actionLANG_JAVA, createObject<QsciLexer, QsciLexerJava> },
        { ui->actionLANG_JAVASCRIPT, createObject<QsciLexer, QsciLexerJavaScript> },
        { ui->actionLANG_JSP, createObject<QsciLexer, QsciLexerJavaScript> },
        { ui->actionLANG_KIX, NULL },
        { ui->actionLANG_LISP, NULL },
        { ui->actionLANG_LUA, createObject<QsciLexer, QsciLexerLua> },
        { ui->actionLANG_MAKEFILE, createObject<QsciLexer, QsciLexerMakefile> },
        { ui->actionLANG_MATLAB, createObject<QsciLexer, QsciLexerMatlab> },
        { ui->actionLANG_MS_INI, NULL },
        { ui->actionLANG_MSDOS, NULL },
        { ui->actionLANG_NORMAL_TEXT, NULL },
        { ui->actionLANG_NSIS, NULL },
        { ui->actionLANG_OBJECTIVE_C, NULL },
        { ui->actionLANG_PASCAL, createObject<QsciLexer, QsciLexerPascal> },
        { ui->actionLANG_PERL, createObject<QsciLexer, QsciLexerPerl> },
        { ui->actionLANG_PHP, NULL },
        { ui->actionLANG_POSTSCRIPT, createObject<QsciLexer, QsciLexerPostScript> },
        { ui->actionLANG_POWERSHELL, NULL },
        { ui->actionLANG_PROPS, createObject<QsciLexer, QsciLexerProperties> },
        { ui->actionLANG_PYTHON, createObject<QsciLexer, QsciLexerPython> },
        { ui->actionLANG_ANSI_R, NULL },
        { ui->actionLANG_RESOURCE_FILE, NULL },
        { ui->actionLANG_RUBY, createObject<QsciLexer, QsciLexerRuby> },
        { ui->actionLANG_BASH, createObject<QsciLexer, QsciLexerBash> },
        { ui->actionLANG_SCHEME, NULL },
        { ui->actionLANG_SMALLTALK, NULL },
        { ui->actionLANG_SQL, createObject<QsciLexer, QsciLexerSQL> },
        { ui->actionLANG_TCL, createObject<QsciLexer, QsciLexerTCL> },
        { ui->actionLANG_TEX, createObject<QsciLexer, QsciLexerTeX> },
        { ui->actionLANG_VB, NULL },
        { ui->actionLANG_VHDL, createObject<QsciLexer, QsciLexerVHDL> },
        { ui->actionLANG_VERILOG, createObject<QsciLexer, QsciLexerVerilog> },
        { ui->actionLANG_XML, createObject<QsciLexer, QsciLexerXML> },
        { ui->actionLANG_YAML, createObject<QsciLexer, QsciLexerYAML> },
        { ui->actionLANG_USER, NULL },
        { NULL, NULL }
    };
    int i=0;
    while (menuLangActionsMap[i].ptrAction) {
        if(menuLangActionsMap[i].ptrAction->isChecked()) {
            QsciLexer *p=reinterpret_cast<QsciScintilla*>
                    (m_pMdiArea->activeSubWindow()->userData(EUSERDATA_SCINTILLA_TEXT_EDITOR))->lexer();
            reinterpret_cast<QsciScintilla*>
                    (m_pMdiArea->activeSubWindow()->userData(EUSERDATA_SCINTILLA_TEXT_EDITOR))->setLexer(0);
            _DEL_MEM(p);
            reinterpret_cast<QsciScintilla*>
                    (m_pMdiArea->activeSubWindow()->userData(EUSERDATA_SCINTILLA_TEXT_EDITOR))->setLexer(menuLangActionsMap[i].fnFunc());
            break;
        }
        ++i;
    }
}

QMap<QString, STextManager>::iterator QNewMainWindow::findKeyFormAreaSubWindow(QMdiSubWindow *ptrSub) {
    QMap<QString, STextManager>::iterator pFind=m_mapOpenedFiles.end();
    if (NULL == ptrSub) {
        _DEBUG_MSG("no data");
        return pFind;
    }
    for(pFind=m_mapOpenedFiles.begin(); pFind!=m_mapOpenedFiles.end(); ++pFind) {
        if (pFind.value().ptrMdiSubWidget == ptrSub) {
            break;
        }
    }
    return pFind;
}

bool QNewMainWindow::addDocPanel(QString str) {
    bool bRet=false;
    STextManager manager={0};
    if (str.isEmpty()) {
        str.sprintf("%s%d", _NEW_FILE_PREFIX, m_nNewDocNum++);
        manager.pTextEditor=new QsciScintilla;

        QsciLexerCPP *lexer=new QsciLexerCPP(manager.pTextEditor);
        manager.pTextEditor->setLexer(lexer);
        manager.ptrMdiSubWidget=m_pMdiArea->addSubWindow(manager.pTextEditor);
        manager.ptrMdiSubWidget->showMaximized();
        manager.ptrMdiSubWidget->setWindowTitle(str.append(" [*]"));
        manager.ptrMdiSubWidget->setUserData(
            EUSERDATA_SCINTILLA_TEXT_EDITOR,
            reinterpret_cast<QObjectUserData*>(manager.pTextEditor));
        m_mapOpenedFiles[str]=manager;
    }
    else {
        QFile *file=new QFile(str);
        if(!file->open(QFile::ReadWrite)) {
            QMessageBox::warning(this, IDS_FILE_OPEN_FAILED_TITLE,
                                 IDS_FILE_OPEN_FAILED_CONTENT.arg(str).arg(file->errorString()));
            _DEL_MEM(file);
            return bRet;
        }
        ;
        manager.pFile=file;
        manager.pTextEditor=new QsciScintilla;
        _DEBUG_MSG("allocate memory: 0x%x", manager.pTextEditor);

        QApplication::setOverrideCursor(Qt::WaitCursor);
        manager.pTextEditor->show();
        m_pMdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(manager.pTextEditor));
        manager.pTextEditor->read(file);
        QApplication::restoreOverrideCursor();
        manager.ptrMdiSubWidget=m_pMdiArea->addSubWindow(manager.pTextEditor);
        manager.ptrMdiSubWidget->showMaximized();
        manager.ptrMdiSubWidget->setUserData(
            EUSERDATA_SCINTILLA_TEXT_EDITOR,
            reinterpret_cast<QObjectUserData*>(manager.pTextEditor));
        QString filename=str.mid(str.lastIndexOf('/')+1);
        manager.ptrMdiSubWidget->setWindowTitle(filename.append(" [*]"));
        m_mapOpenedFiles[str]=manager;
    }
    connect(manager.pTextEditor, SIGNAL(textChanged()), this, SLOT(slotDocWasModified()));
    bRet=true;
    return bRet;
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

    // initial scintilla text editor
    addDocPanel("");
    this->show();
}

void QNewMainWindow::slotAppCmd(QString qstr) {
    _DEBUG_MSG("%s", qstr.toAscii().data());
    this->showMinimized();
    this->activateWindow();
}

void QNewMainWindow::slotDocWasModified() {
    QList<QMdiSubWindow*> list = m_pMdiArea->subWindowList();
    for (QList<QMdiSubWindow*>::iterator pFind=list.begin(); pFind != list.end(); ++pFind) {
        QsciScintilla* ptrEdit=reinterpret_cast<QsciScintilla*>(
                    (*pFind)->userData(EUSERDATA_SCINTILLA_TEXT_EDITOR));
        (*pFind)->setWindowModified(ptrEdit->isModified());
    }
}
