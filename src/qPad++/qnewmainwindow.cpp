#include "qnewmainwindow.h"
#include "ui_qnewmainwindow.h"
#include "debug.h"
#include "mem.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <Qsci/qsciscintilla.h>

QNewMainWindow::QNewMainWindow(QWidget *parent) :
    BaseMainWindow(parent),
    ui(new Ui::QNewMainWindow)
{
    m_pTextEditor=NULL;
}

QNewMainWindow::~QNewMainWindow()
{
    _DEL_MEM(ui);
    _DEL_MEM(m_pTextEditor);
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

    this->resize(QApplication::desktop()->width()/5*4, QApplication::desktop()->height()/5*4);
    BaseMainWindow::slotCreate();
    // initial scintilla text editor
    m_pTextEditor=new QsciScintilla;
    this->setCentralWidget(m_pTextEditor);

    this->show();
}

void QNewMainWindow::slotAppCmd(QString qstr) {
    _DEBUG_MSG("%s", qstr.toAscii().data());
    this->showMinimized();
    this->activateWindow();
}
