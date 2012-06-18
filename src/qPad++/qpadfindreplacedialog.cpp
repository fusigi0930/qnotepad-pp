#include "qpadfindreplacedialog.h"
#include "ui_qpadfindreplacedialog.h"
#include "mem.h"
#include "debug.h"
#include <QTimer>
#include <QCheckBox>
#include <QRadioButton>
#include "qpadmdisubwindow.h"
#include <Qsci/qsciscintilla.h>

QPadFindReplaceDialog::QPadFindReplaceDialog(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::QPadFindReplaceDialog)
{
    m_bIsCreated=false;
    m_nInitIndex=0;
    m_nSearchMode=EMODE_NORMAL;
    m_nSearchFeature=0;
    m_nDirection=EDIR_DOWN;
    m_nTransparentMode=ETRANSPARENT_FOCUS;
    m_nTransparentValue=99;
}

QPadFindReplaceDialog::~QPadFindReplaceDialog()
{
    _DEBUG_MSG("+++");
    _DEL_MEM(ui);
}

void QPadFindReplaceDialog::getCommonUiValue() {
    QRadioButton *ptrModeArray[] = {
        ui->ID_RADIO_SM_NORMAL,
        ui->ID_RADIO_SM_EXTEND,
        ui->ID_RADIO_SM_REGEX,
        NULL
    };
    int i=0;
    while (ptrModeArray[i]) {
        if (ptrModeArray[i]->isChecked()) {
            m_nSearchMode=i;
            break;
        }
        ++i;
    }

    QRadioButton *ptrDirArray[] = {
        ui->ID_RADIO_UP,
        ui->ID_RADIO_DOWN,
        NULL
    };
    i=0;
    while (ptrDirArray[i]) {
        if (ptrDirArray[i]->isChecked()) {
            m_nDirection=i;
            break;
        }
        ++i;
    }

    QRadioButton *ptrTPArray[] = {
        ui->ID_RADIO_ON_LOST_FOCUS,
        ui->ID_RADIO_ALWAYS,
        NULL
    };
    i=0;
    while (ptrTPArray[i]) {
        if (ptrTPArray[i]->isChecked()) {
            m_nTransparentMode=i;
            break;
        }
        ++i;
    }
    m_nTransparentValue=ui->ID_SLIDER_TRANSPARENT->value();
}

void QPadFindReplaceDialog::showEvent(QShowEvent *event) {
    _DEBUG_MSG("+++");
    if (!m_bIsCreated) {
        hide();
        return;
    }
    BaseDialog::showEvent(event);
}

void QPadFindReplaceDialog::closeEvent(QCloseEvent *event) {
    _DEBUG_MSG("+++");
    BaseDialog::closeEvent(event);
    emit sigOnCloseDlg();
}


void QPadFindReplaceDialog::slotCreate() {
    _DEBUG_MSG("+++");
    ui->setupUi(this);

    connect(ui->ID_TAB, SIGNAL(currentChanged(int)), this , SLOT(slotChangeTab(int)));
    connect(ui->ID_BUTTON_FIND_NEXT_FIND, SIGNAL(clicked()), this, SLOT(slotFindFindNext()));

    QTimer::singleShot(0, this, SLOT(slotInitTab()));
    m_bIsCreated=true;
    show();
}

void QPadFindReplaceDialog::slotFindFindNext() {
    // get the check box value
    m_nSearchFeature=0;
    QCheckBox *ptrFeatureBoxArray[] = {
        ui->ID_CHECK_MATCH_WHOLE_WORD_FIND,
        ui->ID_CHECK_MATCH_CASE_FIND,
        ui->ID_CHECK_WARP_FIND,
        NULL
    };
    int i=0;
    while (ptrFeatureBoxArray[i]) {
        if (Qt::Checked == ptrFeatureBoxArray[i]->checkState()) {
            m_nSearchFeature |= (1 << i);
        }
        ++i;
    }

    getCommonUiValue();

    QPadFindReplaceDialog::SValue value(m_nSearchMode, m_nSearchFeature, m_nDirection, m_nTransparentMode, m_nTransparentValue);

    emit sigFindFindNext(value, ui->ID_COMBO_FIND->currentText());

}

void QPadFindReplaceDialog::slotInitTab() {
    ui->ID_TAB->setCurrentIndex(m_nInitIndex);
    this->setWindowTitle(ui->ID_TAB->tabText(m_nInitIndex));
}

void QPadFindReplaceDialog::slotChangeTab(int nIndex) {
    this->setWindowTitle(ui->ID_TAB->tabText(nIndex));
}
