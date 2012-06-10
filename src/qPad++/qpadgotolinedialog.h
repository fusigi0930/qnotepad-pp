#ifndef QPADGOTOLINEDIALOG_H
#define QPADGOTOLINEDIALOG_H

#include "basedialog.h"

namespace Ui {
class QPadGotoLineDialog;
}

class QPadGotoLineDialog : public BaseDialog
{
    Q_OBJECT
    
public:
    explicit QPadGotoLineDialog(QWidget *parent = 0);
    ~QPadGotoLineDialog();
    
protected slots:
    virtual void slotCreate();

private:
    Ui::QPadGotoLineDialog *ui;
};

#endif // QPADGOTOLINEDIALOG_H
