#ifndef DEBUG_H
#define DEBUG_H
#include <QDebug>

#define _DEBUG_MSG(e,...) \
    qDebug("[%s:%d] " e, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)


#endif // DEBUG_H
