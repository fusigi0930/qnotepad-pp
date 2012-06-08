#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>

class BaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BaseDialog(QWidget *parent = 0);
    virtual ~BaseDialog();
    
signals:
    void sigCreate();
    void sigDestroy();

protected slots:
    virtual void slotCreate();
    virtual void slotDestroy();
    
};

#endif // BASEDIALOG_H
