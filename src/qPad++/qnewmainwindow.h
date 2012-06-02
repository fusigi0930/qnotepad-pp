#ifndef QNEWMAINWINDOW_H
#define QNEWMAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "basemainwindow.h"

namespace Ui {
class QNewMainWindow;
}

class QNewMainWindow : public BaseMainWindow
{
    Q_OBJECT


public:

public:
    explicit QNewMainWindow(QWidget *parent = 0);
    ~QNewMainWindow();

protected:
    virtual void changeEvent(QEvent * event);

protected slots:
    virtual void slotCreate();

public slots:
    void slotAppCmd(QString qstr);
    
private:
    Ui::QNewMainWindow *ui;
};

#endif // QNEWMAINWINDOW_H
