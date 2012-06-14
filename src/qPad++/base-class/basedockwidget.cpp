#include "basedockwidget.h"

BaseDockWidget::BaseDockWidget(QWidget *parent) :
    QDockWidget(parent)
{
    connect(this, SIGNAL(sigCreate()), this, SLOT(slotCreate()), Qt::QueuedConnection);
    connect(this, SIGNAL(sigDestroy()), this, SLOT(slotDestroy()), Qt::QueuedConnection);

    emit sigCreate();
}

BaseDockWidget::~BaseDockWidget() {
    slotDestroy();
}

void BaseDockWidget::slotCreate() {

}

void BaseDockWidget::slotDestroy() {

}
