#include "qpadfindreplacedialog.h"
#include "ui_qpadfindreplacedialog.h"
#include "mem.h"
#include "debug.h"
#include <Qtimer>

QPadFindReplaceDialog::QPadFindReplaceDialog(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::QPadFindReplaceDialog)
{
    m_bIsCreated=false;
    m_nInitIndex=0;
}

QPadFindReplaceDialog::~QPadFindReplaceDialog()
{
    _DEL_MEM(ui);
}

void QPadFindReplaceDialog::showEvent(QShowEvent *event) {
    _DEBUG_MSG("+++");
    if (!m_bIsCreated) {
        hide();
        return;
    }
    BaseDialog::showEvent(event);
}

void QPadFindReplaceDialog::slotCreate() {
    _DEBUG_MSG("+++");
    ui->setupUi(this);

    connect(ui->ID_TAB, SIGNAL(currentChanged(int)), this , SLOT(slotChangeTab(int)));
    QTimer::singleShot(0, this, SLOT(slotInitTab()));
    m_bIsCreated=true;
    show();
}

void QPadFindReplaceDialog::slotInitTab() {
    ui->ID_TAB->setCurrentIndex(m_nInitIndex);
    this->setWindowTitle(ui->ID_TAB->tabText(m_nInitIndex));
}

void QPadFindReplaceDialog::slotChangeTab(int nIndex) {
    this->setWindowTitle(ui->ID_TAB->tabText(nIndex));
}
