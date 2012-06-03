#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0);
    virtual ~BaseWidget();
    
signals:
    void sigCreate();
    void sigDestroy();

protected slots:
    virtual void slotCreate();
    virtual void slotDestroy();
    
};

#endif // BASEWIDGET_H
