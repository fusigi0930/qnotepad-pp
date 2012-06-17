#ifndef QPADFINDREPLACEDIALOG_H
#define QPADFINDREPLACEDIALOG_H

#include "basedialog.h"

namespace Ui {
class QPadFindReplaceDialog;
}

class QPadFindReplaceDialog : public BaseDialog
{
    Q_OBJECT
    
private:
    bool m_bIsCreated;

public:
    int m_nInitIndex;

public:
    explicit QPadFindReplaceDialog(QWidget *parent = 0);
    virtual ~QPadFindReplaceDialog();

protected:
    void showEvent(QShowEvent *event);

signals:

private slots:
    void slotChangeTab(int nIndex);
    void slotInitTab();

protected slots:
    virtual void slotCreate();

    
private:
    Ui::QPadFindReplaceDialog *ui;
};

#endif // QPADFINDREPLACEDIALOG_H
