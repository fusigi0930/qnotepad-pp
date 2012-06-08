#include "qpadmdisubwindow.h"
#include "debug.h"
#include "mem.h"
#include "constant.h"
#include <typeinfo>
#include <Qsci/qsciscintilla.h>

QPadMdiSubWindow::QPadMdiSubWindow(QWidget *parent) :
    QMdiSubWindow(parent)
{
}

QPadMdiSubWindow::~QPadMdiSubWindow() {
    _DEBUG_MSG("+++++++");

}

void QPadMdiSubWindow::closeEvent( QCloseEvent *event ) {
    _DEBUG_MSG("close event");
    QWidget *p=widget();
    QsciScintilla *pEdit=reinterpret_cast<QsciScintilla*>(p);
    p->close();
    _DEL_MEM(p);
    emit sigCloseSubWindow(this);
}
