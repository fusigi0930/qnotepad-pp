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

protected slots:
    virtual void slotCreate();
    
private:
    Ui::QNewMainWindow *ui;
};

#endif // QNEWMAINWINDOW_H
