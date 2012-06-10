#include "qpadgotolinedialog.h"
#include "ui_qpadgotolinedialog.h"

QPadGotoLineDialog::QPadGotoLineDialog(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::QPadGotoLineDialog)
{

}

QPadGotoLineDialog::~QPadGotoLineDialog()
{
    delete ui;
}

void QPadGotoLineDialog::slotCreate() {
    ui->setupUi(this);
}
