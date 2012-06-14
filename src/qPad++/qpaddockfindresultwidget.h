#ifndef QPADDOCKFINDRESLUTWIDGET_H
#define QPADDOCKFINDRESLUTWIDGET_H

#include "basedockwidget.h"
#include <QListWidget>
#include <QTreeWidget>

class QPadDockFindResultWidget : public BaseDockWidget
{
    Q_OBJECT

private:
    QListWidget *m_pList;
    QTreeWidget *m_pTree;

public:
    explicit QPadDockFindResultWidget(QWidget *parent = 0);
    virtual ~QPadDockFindResultWidget();

signals:

protected slots:
    virtual void slotCreate();
    virtual void slotDestroy();

};

#endif // QPADDOCKFINDRESLUTWIDGET_H
