#ifndef QPADMDISUBWINDOW_H
#define QPADMDISUBWINDOW_H

#include <QMdiSubWindow>
#include <QCloseEvent>

class QPadMdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit QPadMdiSubWindow(QWidget *parent = 0);
    virtual ~QPadMdiSubWindow();

protected:
    virtual void closeEvent( QCloseEvent *event );
    
signals:
    
public slots:
    
};

#endif // QPADMDISUBWINDOW_H
