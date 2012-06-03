#include "qnewmainwindow.h"
#include "ui_qnewmainwindow.h"
#include "debug.h"
#include "mem.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <Qsci/qsciscintilla.h>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>
#include <QThread>
#include "strings.h"

QNewMainWindow::QNewMainWindow(QWidget *parent) :
    BaseMainWindow(parent),
    ui(new Ui::QNewMainWindow)
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

bool QNewMainWindow::addDocPanel(QString str) {
    bool bRet=false;
    STextManager manager={0};
    if (str.isEmpty()) {
        str.sprintf("%s%d", _NEW_FILE_PREFIX, m_nNewDocNum++);
        manager.pTextEditor=new QsciScintilla;

        manager.ptrMdiSubWidget=m_pMdiArea->addSubWindow(manager.pTextEditor);
        manager.ptrMdiSubWidget->showMaximized();
        manager.ptrMdiSubWidget->setWindowTitle(str);
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
        QString filename=str.mid(str.lastIndexOf('/')+1);
        manager.ptrMdiSubWidget->setWindowTitle(filename);
        m_mapOpenedFiles[str]=manager;
    }
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
