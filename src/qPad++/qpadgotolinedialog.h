#ifndef QPADGOTOLINEDIALOG_H
#define QPADGOTOLINEDIALOG_H

#include "basedialog.h"
#include <QEvent>
#include <QShowEvent>

namespace Ui {
class QPadGotoLineDialog;
}

class QPadGotoLineDialog : public BaseDialog
{
    Q_OBJECT

public:
    enum ETYPE {
        ETYPE_LINE,
        ETYPE_OFFSET
    };

 public:
    int m_nMaxLine;
    int m_nCurrentLine;
    int m_nMaxOffset;
    int m_nCurrentOffset;
    int m_nType;
    
public:
    explicit QPadGotoLineDialog(QWidget *parent = 0);
    ~QPadGotoLineDialog();

protected:
    virtual void changeEvent(QEvent * event);
    
private slots:
    void actionLine(bool bChecked);
    void actionOffset(bool bChecked);

protected slots:
    virtual void slotCreate();
    virtual void accept();

private:
    Ui::QPadGotoLineDialog *ui;
};

#endif // QPADGOTOLINEDIALOG_H
