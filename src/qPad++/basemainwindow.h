#ifndef BASEMAINWINDOW_H
#define BASEMAINWINDOW_H

#include <QMainWindow>
#include "constant.h"

class BaseMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit BaseMainWindow(QWidget *parent = 0);

    virtual ~BaseMainWindow();

protected:
    int     m_nStatus;
    
signals:
    void sigCreate();
    void sigDestroy();

protected slots:
    virtual void slotCreate();
    virtual void slotDestroy();
    
};

#endif // BASEMAINWINDOW_H
