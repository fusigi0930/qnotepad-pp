#include <QtGui/QApplication>
#include "QtSingleApplication.h"
#include <QString>
#include "qnewmainwindow.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);

    if (a.isRunning()) {
        return 0;
    }

    QNewMainWindow w;
    a.setActivationWindow(&w);
    _DEBUG_MSG("set argument: %d", argc);

    return a.exec();
}
