#include "qpadfindreplacedialog.h"
#include "ui_qpadfindreplacedialog.h"
#include "mem.h"
#include "debug.h"
#include <QTimer>
#include <QCheckBox>
#include <QRadioButton>
#include "qpadmdisubwindow.h"
#include <Qsci/qsciscintilla.h>
#include "qnewmainwindow.h"

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
    connect(ui->ID_SLIDER_TRANSPARENT, SIGNAL(valueChanged(int)), this, SLOT(slotOnTransparentSlider(int)));

    QTimer::singleShot(0, this, SLOT(slotInitTab()));

    setWindowOpacity(1.0);

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

    QNewMainWindow *ptrMainWin=qobject_cast<QNewMainWindow*>(parent());
    if (!ptrMainWin) return;
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(ptrMainWin->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    int nLine=-1, nIndex=-1;
    ptrEdit->getCursorPosition(&nLine, &nIndex);
    _DEBUG_MSG("line: %d, index: %d\n", nLine, nIndex);

    if (m_nDirection == QPadFindReplaceDialog::EDIR_UP) {
        int nBLine, nELine, nBIndex, nEIndex;
        ptrEdit->getSelection(&nBLine, &nBIndex, &nELine, &nEIndex);
        nIndex= (nIndex - (nEIndex - nBIndex) < 0 ? 0 : nIndex - (nEIndex - nBIndex));
    }

    bool bResult=ptrEdit->findFirst(ui->ID_COMBO_FIND->currentText(), m_nSearchMode == QPadFindReplaceDialog::EMODE_REGEX,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_CASE) == QPadFindReplaceDialog::EFEATURE_CASE,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_WHOLE_WORD) == QPadFindReplaceDialog::EFEATURE_WHOLE_WORD,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_WARP) == QPadFindReplaceDialog::EFEATURE_WARP,
                       m_nDirection == QPadFindReplaceDialog::EDIR_DOWN, nLine, nIndex, true, true);

}

void QPadFindReplaceDialog::slotFindCount() {

}

void QPadFindReplaceDialog::slotInitTab() {
    ui->ID_TAB->setCurrentIndex(m_nInitIndex);
    this->setWindowTitle(ui->ID_TAB->tabText(m_nInitIndex));
}

void QPadFindReplaceDialog::slotChangeTab(int nIndex) {
    this->setWindowTitle(ui->ID_TAB->tabText(nIndex));
}

void QPadFindReplaceDialog::slotOnTransparentSlider(int nValue) {
    if (0 == nValue) {
        setWindowOpacity(0.3);
        return;
    }
    float fValue=static_cast<float>(nValue+30 > 100 ? 100 : nValue+30)/100;
    setWindowOpacity(fValue);
}
