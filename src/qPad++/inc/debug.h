#ifndef DEBUG_H
#define DEBUG_H
#include <QDebug>

#ifdef _DEBUG_MODE
#   define _DEBUG_MSG(e,...) \
        qDebug("[%s:%d] " e, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#   define _DEBUG_MSG //
#endif

#endif // DEBUG_H
