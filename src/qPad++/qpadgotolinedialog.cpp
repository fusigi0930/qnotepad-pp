#include "qpadgotolinedialog.h"
#include "ui_qpadgotolinedialog.h"
#include "debug.h"
#include "mem.h"
#include <QString>
#include <QTimer>

QPadGotoLineDialog::QPadGotoLineDialog(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::QPadGotoLineDialog)
{
    m_nMaxLine=0;
    m_nCurrentLine=0;
    m_nMaxOffset=0;
    m_nCurrentOffset=0;
    m_nType=QPadGotoLineDialog::ETYPE_LINE;
}

QPadGotoLineDialog::~QPadGotoLineDialog()
{
    _DEL_MEM(ui);
}

void QPadGotoLineDialog::changeEvent(QEvent * event) {
    BaseDialog::changeEvent(event);
    switch(event->type()) {
        default:
            break;
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
    }
}

void QPadGotoLineDialog::slotCreate() {
    _DEBUG_MSG("+++");
    ui->setupUi(this);

    ui->ID_RADIO_BUTTON_LINE->setChecked(true);

    QString qstr;
    qstr.sprintf("%d", m_nMaxLine);
    ui->ID_LABEL_MAX->setText(qstr);
    qstr.sprintf("%d", m_nCurrentLine+1);
    ui->ID_LABEL_NOW_LINE->setText(qstr);

    qstr.clear();
    int nMax=m_nMaxLine;
    while (nMax) {
        qstr.append("0");
        nMax/=10;
    }
    ui->ID_EDIT_GOTO->setInputMask(qstr);

    connect(ui->ID_RADIO_BUTTON_LINE, SIGNAL(clicked(bool)), this, SLOT(actionLine(bool)));
    connect(ui->ID_RADIO_BUTTON_OFFSET, SIGNAL(clicked(bool)), this, SLOT(actionOffset(bool)));

    QTimer::singleShot(0, ui->ID_EDIT_GOTO, SLOT(setFocus()));
}

void QPadGotoLineDialog::actionLine(bool bChecked) {
    _DEBUG_MSG("+++");
    m_nType=QPadGotoLineDialog::ETYPE_LINE;
    QString qstr;
    qstr.sprintf("%d", m_nMaxLine);
    ui->ID_LABEL_MAX->setText(qstr);
    qstr.sprintf("%d", m_nCurrentLine+1);
    ui->ID_LABEL_NOW_LINE->setText(qstr);

    qstr.clear();
    int nMax=m_nMaxLine;
    while (nMax) {
        qstr.append("0");
        nMax/=10;
    }
    ui->ID_EDIT_GOTO->setInputMask(qstr);
    ui->ID_EDIT_GOTO->clear();
    ui->ID_EDIT_GOTO->setCursorPosition(0);
    ui->ID_EDIT_GOTO->setFocus();

}

void QPadGotoLineDialog::actionOffset(bool bChecked) {
    _DEBUG_MSG("+++");
    m_nType=QPadGotoLineDialog::ETYPE_OFFSET;
    QString qstr;
    qstr.sprintf("%d", m_nMaxOffset);
    ui->ID_LABEL_MAX->setText(qstr);
    qstr.sprintf("%d", m_nCurrentOffset);
    ui->ID_LABEL_NOW_LINE->setText(qstr);

    qstr.clear();
    int nMax=m_nMaxOffset;
    while (nMax) {
        qstr.append("0");
        nMax/=10;
    }
    ui->ID_EDIT_GOTO->setInputMask(qstr);
    ui->ID_EDIT_GOTO->clear();
    ui->ID_EDIT_GOTO->setCursorPosition(0);
    ui->ID_EDIT_GOTO->setFocus();
}

void QPadGotoLineDialog::accept() {
    if (ui->ID_EDIT_GOTO->text().isEmpty()) {
        m_nCurrentOffset=m_nCurrentLine=-1;
    }
    else {
        m_nCurrentOffset=m_nCurrentLine=ui->ID_EDIT_GOTO->text().toInt();
    }
    QDialog::accept();
}
