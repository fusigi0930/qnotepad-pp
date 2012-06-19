#include "qpaddockfindresultwidget.h"
#include "mem.h"
#include "debug.h"
#include "constant.h"
#include "strings.h"
#include <QStringList>
#include "qpadmdisubwindow.h"
#include <Qsci/qsciscintilla.h>
#include <Scintilla.h>
#include <QMdiArea>
#include <QVariant>
#include <QString>

QPadDockFindResultWidget::QPadDockFindResultWidget(QWidget *parent) :
    BaseDockWidget(parent)
{
    m_pList=NULL;
    m_pTree=NULL;
    setWindowTitle(IDS_TITLE_FIND_RESULT);
    m_pTree=new QTreeWidget(this);
    setAllowedAreas(Qt::BottomDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);
    setWidget(m_pTree);
    m_pTree->setHeaderLabel(IDS_TITLE_FIND_RESULT);

    connect(m_pTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(slotDoubleClick(QTreeWidgetItem*, int)));
}

QPadDockFindResultWidget::~QPadDockFindResultWidget() {
    _DEBUG_MSG("+++");
    QPadDockFindResultWidget::slotDestroy();
}

void QPadDockFindResultWidget::closeEvent(QCloseEvent *event) {
    BaseDockWidget::closeEvent(event);
    emit sigOnClose();
}

void QPadDockFindResultWidget::slotDoubleClick(QTreeWidgetItem* item, int nColumn) {
    _DEBUG_MSG("+++");
    QPadMdiSubWindow *ptrSubWin=reinterpret_cast<QPadMdiSubWindow *>(item->data(0, EROLE_SUB_WIN).toLongLong());
    if (!ptrSubWin) return;

    int nPos=qvariant_cast<int>(item->data(0, EROLE_FIND_POS));

    emit sigSetSubWinActivate(ptrSubWin);

    QsciScintilla* ptrEdit=reinterpret_cast<QsciScintilla*>(ptrSubWin->widget());
    if (!ptrEdit) return;

    ptrEdit->SendScintilla(SCI_GOTOPOS, nPos);
    ptrEdit->setFocus();
}

void QPadDockFindResultWidget::slotCreate() {

}

void QPadDockFindResultWidget::slotDestroy() {
    setWidget(NULL);
    _DEL_MEM(m_pList);
    _DEL_MEM(m_pTree);
}

QTreeWidgetItem *QPadDockFindResultWidget::slotAddRootItem(QString itemName) {
    QTreeWidgetItem *pRet=NULL;
    pRet=new QTreeWidgetItem(QStringList(QString("Search \"%1\"").arg(itemName)));
    _DEBUG_MSG("pRet: 0x%x", pRet);
    pRet->setTextColor(0, QColor(0, 0, 150));
    pRet->setBackgroundColor(0, QColor(210, 210, 250));
    return pRet;
}

void QPadDockFindResultWidget::slotAddItem(QTreeWidgetItem *root) {
    if (!root)
        return;
    m_pTree->collapseAll();
    m_pTree->insertTopLevelItem(0, root);

    m_pTree->expandItem(root);
    _DEBUG_MSG("child count: %d", root->childCount());

    for (int i=0; i<root->childCount(); ++i)
        m_pTree->expandItem(root->child(i));
}
