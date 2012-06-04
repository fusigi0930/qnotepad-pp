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
#include "mem.h"
#include <QAction>
#include <QActionGroup>

namespace Ui {
class QNewMainWindow;
}

struct STextManager {
    QFile *pFile;
    QsciScintilla *pTextEditor;
    QMdiSubWindow *ptrMdiSubWidget;
};

template<typename T_T, typename O_O>
T_T *createObject() { return new O_O; }

template<typename T_T>
struct SActionMap {
    QAction *ptrAction;
    T_T* (*fnFunc)();
};

class QNewMainWindow : public BaseMainWindow
{
    Q_OBJECT

private:
    QMdiArea *m_pMdiArea;
    QMap<QString, STextManager> m_mapOpenedFiles;
    QActionGroup m_langActionsGroup;

    int m_nNewDocNum;

public:
    explicit QNewMainWindow(QWidget *parent = 0);
    ~QNewMainWindow();

protected:
    virtual void changeEvent(QEvent * event);

private:
    void setMenuActions();
    bool addDocPanel(QString str);
    QMap<QString, STextManager>::iterator findKeyFormAreaSubWindow(QMdiSubWindow *ptrSub);

private slots:
    // ui action's mapping functaions
    void actionFileOpen();
    void actionFileNew();
    void actionLang();

protected slots:
    virtual void slotCreate();

public slots:
    void slotAppCmd(QString qstr);
    void slotDocWasModified();
    
private:
    Ui::QNewMainWindow *ui;
};

#endif // QNEWMAINWINDOW_H
