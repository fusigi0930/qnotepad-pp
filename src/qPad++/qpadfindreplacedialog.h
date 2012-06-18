#ifndef QPADFINDREPLACEDIALOG_H
#define QPADFINDREPLACEDIALOG_H

#include "basedialog.h"

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
    struct SValue {
        int nMode;
        int nFeature;
        int nDirection;
        int nTransMode;
        int nTransValue;

        SValue() { }
        SValue(int mode, int feature, int direction, int transMode, int transValue) {
            nMode=mode;
            nFeature=feature;
            nDirection=direction;
            nTransMode=transMode;
            nTransValue=transValue;
        }
        SValue& operator=(const SValue& other) {
            nMode=other.nMode;
            nFeature=other.nFeature;
            nDirection=other.nDirection;
            nTransMode=other.nTransMode;
            nTransValue=other.nTransValue;
        }
    };

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

signals:
    void sigFindFindNext(QPadFindReplaceDialog::SValue, QString);
    void sigOnCloseDlg();

private slots:
    void slotChangeTab(int nIndex);
    void slotInitTab();
    void slotFindFindNext();
    void slotOnTransparentSlider(int nValue);

protected slots:
    virtual void slotCreate();

    
private:
    Ui::QPadFindReplaceDialog *ui;
};

#endif // QPADFINDREPLACEDIALOG_H
