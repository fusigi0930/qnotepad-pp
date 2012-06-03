#include "basemainwindow.h"
#include "debug.h"

BaseMainWindow::BaseMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    connect(this, SIGNAL(sigCreate()), this, SLOT(slotCreate()), Qt::QueuedConnection);
    connect(this, SIGNAL(sigDestroy()), this, SLOT(slotDestroy()), Qt::QueuedConnection);

    m_nStatus=ESTATUS_UNKNOW;
    emit sigCreate();
}

BaseMainWindow::~BaseMainWindow() {
    slotDestroy();
}

void BaseMainWindow::slotCreate() {
    m_nStatus=ESTATUS_CREATED;
}

void BaseMainWindow::slotDestroy() {
    m_nStatus=ESTATUS_DESTORYED;
}
