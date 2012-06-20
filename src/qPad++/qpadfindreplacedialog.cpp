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
#include <Scintilla.h>
#include <QMessageBox>
#include "strings.h"

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

void QPadFindReplaceDialog::getFindTabValue() {
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
}

void QPadFindReplaceDialog::getReplaceTabValue() {
    // get the check box value
    m_nSearchFeature=0;
    QCheckBox *ptrFeatureBoxArray[] = {
        ui->ID_CHECK_MATCH_WHOLE_WORD_REPLACE,
        ui->ID_CHECK_MATCH_CASE_REPLACE,
        ui->ID_CHECK_WARP_REPLACE,
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
}

void QPadFindReplaceDialog::getFFTabValue() {
    getCommonUiValue();
}

void QPadFindReplaceDialog::getMarkTabValue() {
    getCommonUiValue();
}

void QPadFindReplaceDialog::findAllinSubWin(QTreeWidgetItem *root, QMdiSubWindow *ptrWin) {
    QPadMdiSubWindow *ptrSubWin=dynamic_cast<QPadMdiSubWindow*>(ptrWin);
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    int nCurPos=ptrEdit->SendScintilla(SCI_GETCURRENTPOS);

    bool bResult=ptrEdit->findFirst(ui->ID_COMBO_FIND->currentText(), m_nSearchMode == QPadFindReplaceDialog::EMODE_REGEX,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_CASE) == QPadFindReplaceDialog::EFEATURE_CASE,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_WHOLE_WORD) == QPadFindReplaceDialog::EFEATURE_WHOLE_WORD,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_WARP) == QPadFindReplaceDialog::EFEATURE_WARP,
                       true, 0, 0, false, false);

    if (!bResult) {
        return;
    }

    int nLine=-1, nIndex=-1;
    ptrEdit->getCursorPosition(&nLine, &nIndex);
    QTreeWidgetItem *ptrRoot=root;
    _DEBUG_MSG("ptrRoot: 0x%x", ptrRoot);
    if (!ptrRoot) return;

    QTreeWidgetItem* pFile=new QTreeWidgetItem(ptrRoot, QStringList(ptrSubWin->m_qstrFileName));
    pFile->setTextColor(0, QColor(0, 150, 0));
    pFile->setBackgroundColor(0, QColor(210, 250, 210));
    ptrRoot->addChild(pFile);

    while (bResult) {
        int nNextLine, nNextIndex;
        ptrEdit->getCursorPosition(&nNextLine, &nNextIndex);

        int nPos=ptrEdit->SendScintilla(SCI_GETCURRENTPOS);

        QTreeWidgetItem *pItem=new QTreeWidgetItem(pFile,
                                   QStringList(QString("Line %1: %2")
                                            .arg(nNextLine)
                                            .arg(ptrEdit->text(nNextLine).replace("\r","").replace("\n", ""))));
        pItem->setData(0, EROLE_SUB_WIN, reinterpret_cast<qlonglong>(ptrSubWin));
        pItem->setData(0, EROLE_FIND_POS, nPos);
        pFile->addChild(pItem);

        bResult=ptrEdit->findNext();
        ptrEdit->getCursorPosition(&nNextLine, &nNextIndex);
        if (nNextLine == nLine && nNextIndex == nIndex)
            break;
    }

    emit sigInsertRootInResultWin(ptrRoot);
    ptrEdit->SendScintilla(SCI_GOTOPOS, nCurPos);
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

bool QPadFindReplaceDialog::event(QEvent *event) {
    switch (event->type()) {
        default: break;
    case QEvent::WindowActivate:
        if (!m_bIsCreated || ui->ID_RADIO_ALWAYS->isChecked()) {
            break;
        }
        disconnect(ui->ID_SLIDER_TRANSPARENT, SIGNAL(valueChanged(int)), this, SLOT(slotOnTransparentSlider(int)));
        setWindowOpacity(1.0);
        break;
    case QEvent::WindowDeactivate:
        if (!m_bIsCreated || !ui->ID_GROUP_TRANSPARENT->isChecked()) {
            break;
        }
        connect(ui->ID_SLIDER_TRANSPARENT, SIGNAL(valueChanged(int)), this, SLOT(slotOnTransparentSlider(int)));
        slotOnTransparentSlider(ui->ID_SLIDER_TRANSPARENT->value());
        break;
    }

    return BaseDialog::event(event);
}

void QPadFindReplaceDialog::slotCreate() {
    _DEBUG_MSG("+++");
    ui->setupUi(this);

    connect(ui->ID_TAB, SIGNAL(currentChanged(int)), this , SLOT(slotChangeTab(int)));

    connect(ui->ID_BUTTON_FIND_NEXT_FIND, SIGNAL(clicked()), this, SLOT(slotFindFindNext()));
    connect(ui->ID_BUTTON_COUNT_FIND, SIGNAL(clicked()), this, SLOT(slotFindCount()));
    connect(ui->ID_BUTTON_CLOSE_FIND, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->ID_BUTTON_FA_IN_CUR_FIND, SIGNAL(clicked()), this, SLOT(slotFindFindAllCurrent()));
    connect(ui->ID_BUTTON_FA_IN_ALL_FIND, SIGNAL(clicked()), this, SLOT(slotFindFindAllinOpen()));

    connect(ui->ID_BUTTON_CLOSE_REPLACE, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->ID_BUTTON_FIND_NEXT_REPLACE, SIGNAL(clicked()), this, SLOT(slotReplaceFindNext()));

    connect(ui->ID_BUTTON_CLOSE_FILES, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->ID_BUTTON_CLOSE_MARK, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->ID_RADIO_ALWAYS, SIGNAL(clicked(bool)), this, SLOT(slotOnAlwaysTransparent(bool)));
    connect(ui->ID_RADIO_ON_LOST_FOCUS, SIGNAL(clicked(bool)), this, SLOT(slotOnFocusTransparent(bool)));
    connect(ui->ID_GROUP_TRANSPARENT, SIGNAL(clicked(bool)), this, SLOT(slotOnClickTransparentGroup(bool)));

    QTimer::singleShot(0, this, SLOT(slotInitTab()));

    setWindowOpacity(1.0);

    m_bIsCreated=true;
    show();
}

void QPadFindReplaceDialog::slotFindFindNext() {
    getFindTabValue();

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

    if (bResult) {
        ptrEdit->setFocus();
    }
}

void QPadFindReplaceDialog::slotFindCount() {
    getFindTabValue();
    int nCount=0;

    QNewMainWindow *ptrMainWin=qobject_cast<QNewMainWindow*>(parent());
    if (!ptrMainWin) return;
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(ptrMainWin->getMdiActiveWindow());
    if (!ptrSubWin) return;
    QsciScintilla *ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    int nPos=ptrEdit->SendScintilla(SCI_GETCURRENTPOS);

    bool bResult=ptrEdit->findFirst(ui->ID_COMBO_FIND->currentText(), m_nSearchMode == QPadFindReplaceDialog::EMODE_REGEX,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_CASE) == QPadFindReplaceDialog::EFEATURE_CASE,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_WHOLE_WORD) == QPadFindReplaceDialog::EFEATURE_WHOLE_WORD,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_WARP) == QPadFindReplaceDialog::EFEATURE_WARP,
                       true, 0, 0, false, false);

    if (!bResult) {
        return;
    }

    int nLine=-1, nIndex=-1;
    ptrEdit->getCursorPosition(&nLine, &nIndex);

    while (bResult) {
        ++nCount;
        bResult=ptrEdit->findNext();
        int nNextLine, nNextIndex;
        ptrEdit->getCursorPosition(&nNextLine, &nNextIndex);
        if (nNextLine == nLine && nNextIndex == nIndex)
            break;
    }

    ptrEdit->SendScintilla(SCI_GOTOPOS, nPos);
    ptrEdit->setFocus();
    _DEBUG_MSG("total count: %d", nCount);
    QMessageBox::information(this, IDS_TITLE_FIND_COUNT, IDS_FIND_COUNT.arg(nCount), QMessageBox::Ok);
}

void QPadFindReplaceDialog::slotFindFindAllCurrent() {
    getFindTabValue();
    QNewMainWindow *ptrMainWin=qobject_cast<QNewMainWindow*>(parent());
    if (!ptrMainWin) return;
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow*>(ptrMainWin->getMdiActiveWindow());
    if (!ptrSubWin) return;

    emit sigOnCreateFindReslutWidget();

    QTreeWidgetItem *ptrRoot=emit sigOnCreateRootItemInResultWin(ui->ID_COMBO_FIND->currentText());
    _DEBUG_MSG("ptrRoot: 0x%x", ptrRoot);

    findAllinSubWin(ptrRoot, ptrSubWin);
}

void QPadFindReplaceDialog::slotFindFindAllinOpen() {
    getFindTabValue();
    QNewMainWindow *ptrMainWin=qobject_cast<QNewMainWindow*>(parent());
    if (!ptrMainWin) return;

    QMdiArea *ptrArea=ptrMainWin->getMdiArea();
    if (!ptrArea) return;

    emit sigOnCreateFindReslutWidget();

    QTreeWidgetItem *ptrRoot=emit sigOnCreateRootItemInResultWin(ui->ID_COMBO_FIND->currentText());
    _DEBUG_MSG("ptrRoot: 0x%x", ptrRoot);

    QList<QMdiSubWindow*> subList=ptrArea->subWindowList();
    for (QList<QMdiSubWindow*>::iterator pSubWin=subList.begin(); pSubWin != subList.end(); ++pSubWin) {
        findAllinSubWin(ptrRoot, *pSubWin);
    }

}

void QPadFindReplaceDialog::slotReplaceFindNext() {
    getReplaceTabValue();

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

    bool bResult=ptrEdit->findFirst(ui->ID_COMBO_R_FIND->currentText(), m_nSearchMode == QPadFindReplaceDialog::EMODE_REGEX,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_CASE) == QPadFindReplaceDialog::EFEATURE_CASE,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_WHOLE_WORD) == QPadFindReplaceDialog::EFEATURE_WHOLE_WORD,
                       (m_nSearchFeature & QPadFindReplaceDialog::EFEATURE_WARP) == QPadFindReplaceDialog::EFEATURE_WARP,
                       m_nDirection == QPadFindReplaceDialog::EDIR_DOWN, nLine, nIndex, true, true);

    if (bResult) {
        ptrEdit->setFocus();
    }
}

void QPadFindReplaceDialog::slotInitTab() {
    ui->ID_TAB->setCurrentIndex(m_nInitIndex);
    this->setWindowTitle(ui->ID_TAB->tabText(m_nInitIndex));

    switch (m_nInitIndex) {
        default: break;
        case EFUNC_FIND:
            ui->ID_COMBO_FIND->setFocus();
            break;
        case EFUNC_REPLACE:
            ui->ID_COMBO_R_FIND->setFocus();
            break;
        case EFUNC_FIND_IN_FILES:
            ui->ID_COMBO_FIND_FILES->setFocus();
            break;
        case EFUNC_MARK:
            ui->ID_COMBO_MARK->setFocus();
            break;
    }
}

void QPadFindReplaceDialog::slotChangeTab(int nIndex) {
    this->setWindowTitle(ui->ID_TAB->tabText(nIndex));
}

void QPadFindReplaceDialog::slotOnTransparentSlider(int nValue) {
    float fValue=static_cast<float>(nValue)/100.0 * 0.7 + 0.3;
    setWindowOpacity(fValue);
}

void QPadFindReplaceDialog::slotOnAlwaysTransparent(bool bChecked) {
    if (!bChecked) return;

    connect(ui->ID_SLIDER_TRANSPARENT, SIGNAL(valueChanged(int)), this, SLOT(slotOnTransparentSlider(int)));
    slotOnTransparentSlider(ui->ID_SLIDER_TRANSPARENT->value());

}

void QPadFindReplaceDialog::slotOnFocusTransparent(bool bChecked) {
    if (!bChecked) return;

    setWindowOpacity(1.0);
}

void QPadFindReplaceDialog::slotOnClickTransparentGroup(bool bChecked) {
    if (bChecked) {
        slotOnFocusTransparent(ui->ID_RADIO_ON_LOST_FOCUS->isChecked());
        slotOnAlwaysTransparent(ui->ID_RADIO_ALWAYS->isChecked());
    }
    else {
        disconnect(ui->ID_SLIDER_TRANSPARENT, SIGNAL(valueChanged(int)), this, SLOT(slotOnTransparentSlider(int)));
        setWindowOpacity(1.0);
    }
}
