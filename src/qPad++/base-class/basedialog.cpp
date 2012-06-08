#include "basedialog.h"

BaseDialog::BaseDialog(QWidget *parent) :
    QDialog(parent)
{
    connect(this, SIGNAL(sigCreate()), this, SLOT(slotCreate()), Qt::QueuedConnection);
    connect(this, SIGNAL(sigDestroy()), this, SLOT(slotDestroy()), Qt::QueuedConnection);

    emit sigCreate();
}

BaseDialog::~BaseDialog() {
    slotDestroy();
}

void BaseDialog::slotCreate() {

}

void BaseDialog::slotDestroy() {

}
