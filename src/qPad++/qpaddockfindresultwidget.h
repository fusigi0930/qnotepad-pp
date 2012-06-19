#ifndef QPADDOCKFINDRESLUTWIDGET_H
#define QPADDOCKFINDRESLUTWIDGET_H

#include "basedockwidget.h"
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMdiSubWindow>

class QPadDockFindResultWidget : public BaseDockWidget
{
    Q_OBJECT

private:
    QListWidget *m_pList;
    QTreeWidget *m_pTree;

public:
    explicit QPadDockFindResultWidget(QWidget *parent = 0);
    virtual ~QPadDockFindResultWidget();

protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void sigDoubleClick(QModelIndex);
    void sigOnClose();
    void sigSetSubWinActivate(QMdiSubWindow *);

private slots:
    void slotDoubleClick(QTreeWidgetItem* item, int nColumn);

protected slots:
    virtual void slotCreate();
    virtual void slotDestroy();

public slots:
    QTreeWidgetItem *slotAddRootItem(QString itemName);
    void slotAddItem(QTreeWidgetItem *root);

};

#endif // QPADDOCKFINDRESLUTWIDGET_H
