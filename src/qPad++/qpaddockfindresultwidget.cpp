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
    m_pTree=NULL;
}

QPadDockFindResultWidget::~QPadDockFindResultWidget() {
    slotDestroy();
}

void QPadDockFindResultWidget::slotCreate() {
    setWindowTitle(IDS_TITLE_FIND_RESULT);
    m_pTree=new QTreeWidget(this);
    setAllowedAreas(Qt::BottomDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);
    setWidget(m_pTree);

}

void QPadDockFindResultWidget::slotDestroy() {
    setWidget(NULL);
    _DEL_MEM(m_pList);
    _DEL_MEM(m_pTree);
}
