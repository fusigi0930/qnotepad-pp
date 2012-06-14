#include "qpaddockfindresultwidget.h"
#include "mem.h"
#include "debug.h"
#include "constant.h"
#include "strings.h"
#include <QStringList>

QPadDockFindResultWidget::QPadDockFindResultWidget(QWidget *parent) :
    BaseDockWidget(parent)
{
    m_pList=NULL;
}

QPadDockFindResultWidget::~QPadDockFindResultWidget() {
    slotDestroy();
}

void QPadDockFindResultWidget::slotCreate() {
    setWindowTitle(IDS_TITLE_FIND_RESULT);
    m_pList=new QListWidget(this);
    setWidget(m_pList);

    m_pList->addItems(QStringList() << "test1" << "test2" << "test3" << "test4");
}

void QPadDockFindResultWidget::slotDestroy() {
    setWidget(NULL);
    _DEL_MEM(m_pList);
}
