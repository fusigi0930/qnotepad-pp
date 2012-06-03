#ifndef QNEWMAINWINDOW_H
#define QNEWMAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "basemainwindow.h"
#include <Qsci/qsciscintilla.h>
#include <QMap>
#include <QFile>
#include <QMdiArea>
#include <QMdiSubWindow>

namespace Ui {
class QNewMainWindow;
}

struct STextManager {
    QFile *pFile;
    QsciScintilla *pTextEditor;
    QMdiSubWindow *ptrMdiSubWidget;
};

class QNewMainWindow : public BaseMainWindow
{
    Q_OBJECT

private:
    QMdiArea *m_pMdiArea;
    QMap<QString, STextManager> m_mapOpenedFiles;

    int m_nNewDocNum;

public:
    explicit QNewMainWindow(QWidget *parent = 0);
    ~QNewMainWindow();

protected:
    virtual void changeEvent(QEvent * event);

private:
    void setMenuActions();
    bool addDocPanel(QString str);

private slots:
    // ui action's mapping functaions
    void actionFileOpen();
    void actionFileNew();

protected slots:
    virtual void slotCreate();

public slots:
    void slotAppCmd(QString qstr);
    
private:
    Ui::QNewMainWindow *ui;
};

#endif // QNEWMAINWINDOW_H
