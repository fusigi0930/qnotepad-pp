#include "basewidget.h"

BaseWidget::BaseWidget(QWidget *parent) :
    QWidget(parent)
{
    connect(this, SIGNAL(sigCreate()), this, SLOT(slotCreate()), Qt::QueuedConnection);
    connect(this, SIGNAL(sigDestroy()), this, SLOT(slotDestroy()), Qt::QueuedConnection);

    emit sigCreate();
}

BaseWidget::~BaseWidget() {
    BaseWidget::slotDestroy();
}

void BaseWidget::slotCreate() {

}

void BaseWidget::slotDestroy() {

}
