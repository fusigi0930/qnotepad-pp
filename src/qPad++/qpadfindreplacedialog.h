#ifndef QPADFINDREPLACEDIALOG_H
#define QPADFINDREPLACEDIALOG_H

#include "basedialog.h"
#include <QTreeWidgetItem>
#include <QMdiSubWindow>

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

    int m_nSearchMode;
    int m_nSearchFeature;
    int m_nDirection;
    int m_nTransparentMode;
    int m_nTransparentValue;

public:
    enum EInitFunc {
        EFUNC_FIND,
        EFUNC_REPLACE,
        EFUNC_FIND_IN_FILES,
        EFUNC_MARK
    };

    enum ESearchMode {
        EMODE_NORMAL,
        EMODE_EXTEND,
        EMODE_REGEX
    };

    enum ESearchFeature {
        EFEATURE_WHOLE_WORD = 0x01,
        EFEATURE_CASE       = 0x02,
        EFEATURE_WARP       = 0x04
    };

    enum ESearchDirection {
        EDIR_UP,
        EDIR_DOWN
    };

    enum ESearchTransparent {
        ETRANSPARENT_FOCUS,
        ETRANSPARENT_ALWAYS
    };

public:
    explicit QPadFindReplaceDialog(QWidget *parent = 0);
    virtual ~QPadFindReplaceDialog();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);

private:
    void getCommonUiValue();
    void getFindTabValue();
    void findAllinSubWin(QTreeWidgetItem *root, QMdiSubWindow *ptrWin);

signals:
    void sigOnCloseDlg();
    void sigOnCreateFindReslutWidget();
    QTreeWidgetItem* sigOnCreateRootItemInResultWin(QString itemName);
    void sigInsertRootInResultWin(QTreeWidgetItem *root);

private slots:
    void slotChangeTab(int nIndex);
    void slotInitTab();

    void slotFindFindNext();
    void slotFindCount();
    void slotFindFindAllCurrent();
    void slotFindFindAllinOpen();

    void slotOnTransparentSlider(int nValue);

protected slots:
    virtual void slotCreate();

    
private:
    Ui::QPadFindReplaceDialog *ui;
};

#endif // QPADFINDREPLACEDIALOG_H
