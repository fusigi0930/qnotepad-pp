#include <QtGui/QApplication>
#include "QtSingleApplication.h"
#include <QString>
#include <QTranslator>
#include "qnewmainwindow.h"
#include "debug.h"
#include "mem.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);

    if (a.isRunning()) {
        QString qstr;
        qstr.sprintf("open %s", 1 < argc ? argv[1] : "");
        _DEBUG_MSG("cmd: %s", qstr.toAscii().data());
        a.sendMessage(qstr);
        return 0;
    }

    QTranslator *pTranslator=new QTranslator;
    pTranslator->load(":/lang/strings_en.qm");
    a.installTranslator(pTranslator);
    QNewMainWindow w;
    a.setActivationWindow(&w);
    a.connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(slotAppCmd(QString)));

    int nRet = a.exec();
    _DEL_MEM(pTranslator);
    return nRet;
}
