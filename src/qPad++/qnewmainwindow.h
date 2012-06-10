#ifndef QNEWMAINWINDOW_H
#define QNEWMAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "basemainwindow.h"
#include <Qsci/qsciscintilla.h>
#include <QFile>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "mem.h"
#include <QAction>
#include <QActionGroup>
#include <vector>
#include "qpadmdisubwindow.h"
#include <QMap>
#include <QShortcut>

namespace Ui {
class QNewMainWindow;
}

template<typename T_T, typename O_O>
T_T *createObject() { return new O_O; }

template<typename T_T>
struct SActionMap {
    QAction *ptrAction;
    T_T* (*fnFunc)();

    SActionMap(QAction *action, T_T* (*func)()) {ptrAction=action; fnFunc=func;}
};

class QNewMainWindow : public BaseMainWindow
{
    Q_OBJECT

private:
    QMdiArea *m_pMdiArea;
    QActionGroup m_langActionsGroup;
    QMap<unsigned int, QShortcut*> m_mapShortcuts;

    std::vector<SActionMap<QsciLexer> > m_vtMenuLangActions;

    int m_nNewDocNum;

public:
    explicit QNewMainWindow(QWidget *parent = 0);
    ~QNewMainWindow();

protected:
    virtual void changeEvent(QEvent * event);

private:
    void setMenuActions();
    void setFileMenuActions();
    void setEditMenuActions();
    void setLangMenuActions();
    void setSearchMenuActions();
    void setViewMenuActions();
    void setEncodingActions();
    void setSettingsActions();
    void setMacroActions();
    void setRunActions();
    void setHelpActions();

    void setUnmenuActoins();
    void SetScintillaEditMenu(QMdiSubWindow *ptrSubWin);

    bool addDocPanel(QString str);
    QString saveDoc(QString qstrFile, QPadMdiSubWindow *ptrSubWin);
    QPadMdiSubWindow* findSubWinsFilename(QString qstr);

    void setUiMenuItem(QMdiSubWindow *ptrSubWin);
    int closeSubWinFile(QMdiSubWindow *ptrSubWin);

protected:
    QMdiSubWindow* getMdiActiveWindow();

private slots:
    // ui action's mapping functaions
    void actionFileOpen();
    void actionFileNew();
    void actionFileSave();
    void actionFileReload();
    void actionFileSaveAs();
    void actionFileSaveAsCopy();
    void actionFileSaveAll();
    void actionFileClose();
    void actionFileCloseAll();
    void actionFileCloseAllExceptCurrent();
    void actionFileExit();
    void actionFilePrint();
    void actionFilePrintNow();
    void actionFileRename();

    void actionEditUndo();
    void actionEditRedo();
    void actionEditCut();
    void actionEditCopy();
    void actionEditPaste();
    void actionEditDel();
    void actionEditSelectAll();

    void actionLang();

    void actionSearchBookmark();
    void actionSearchBookmarkNext();
    void actionSearchBookmarkPrev();
    void actionSearchGotoLine();

protected slots:
    virtual void slotCreate();

public slots:
    void slotAppCmd(QString qstr);
    void slotDocWasModified();
    void slotOnChangedSubWindow(QMdiSubWindow *ptrSubWin);
    void slotOnCloseSubWindow(QMdiSubWindow *ptrSubWin);
    
private:
    Ui::QNewMainWindow *ui;
};

#endif // QNEWMAINWINDOW_H
