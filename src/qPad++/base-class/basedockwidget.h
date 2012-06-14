#ifndef BASEDOCKWIDGET_H
#define BASEDOCKWIDGET_H

#include <QDockWidget>

class BaseDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit BaseDockWidget(QWidget *parent = 0);
    virtual ~BaseDockWidget();

signals:
    void sigCreate();
    void sigDestroy();
    void sigOnClose(QWidget *ptrWidget);

protected slots:
    virtual void slotCreate();
    virtual void slotDestroy();

};

#endif // BASEDOCKWIDGET_H
