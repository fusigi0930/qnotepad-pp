#ifndef QPADMDISUBWINDOW_H
#define QPADMDISUBWINDOW_H

#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QString>

class QPadMdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    QString m_qstrFileName;
public:
    explicit QPadMdiSubWindow(QWidget *parent = 0);
    virtual ~QPadMdiSubWindow();

protected:
    virtual void closeEvent( QCloseEvent *event );
    
signals:
    void sigCloseSubWindow(QMdiSubWindow*);
    
public slots:
    
};

#endif // QPADMDISUBWINDOW_H
