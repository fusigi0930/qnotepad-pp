#include "qpadfindreplacedialog.h"
#include "ui_qpadfindreplacedialog.h"

QPadFindReplaceDialog::QPadFindReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPadFindReplaceDialog)
{
    ui->setupUi(this);
}

QPadFindReplaceDialog::~QPadFindReplaceDialog()
{
    delete ui;
}
