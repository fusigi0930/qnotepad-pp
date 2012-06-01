#include "qnewmainwindow.h"
#include "ui_qnewmainwindow.h"
#include "debug.h"
#include "mem.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>

QNewMainWindow::QNewMainWindow(QWidget *parent) :
    BaseMainWindow(parent),
    ui(new Ui::QNewMainWindow)
{

}

QNewMainWindow::~QNewMainWindow()
{
    _DEL_MEM(ui);
}

void QNewMainWindow::slotCreate() {
    BaseMainWindow::slotCreate();
    _DEBUG_MSG("%s", __PRETTY_FUNCTION__);
    ui->setupUi(this);

    this->resize(QApplication::desktop()->width()/5*4, QApplication::desktop()->height()/5*4);
    this->show();
}
