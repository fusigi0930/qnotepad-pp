#ifndef QPADFINDREPLACEDIALOG_H
#define QPADFINDREPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class QPadFindReplaceDialog;
}

class QPadFindReplaceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QPadFindReplaceDialog(QWidget *parent = 0);
    ~QPadFindReplaceDialog();
    
private:
    Ui::QPadFindReplaceDialog *ui;
};

#endif // QPADFINDREPLACEDIALOG_H
