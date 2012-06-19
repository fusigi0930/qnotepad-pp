#ifndef CONSTANT_H
#define CONSTANT_H
#include <qnamespace.h>

enum EWINDOW_STATUS {
    ESTATUS_CREATING,
    ESTATUS_CREATED,
    ESTATUS_DESTORYING,
    ESTATUS_DESTORYED,
    ESTATUS_UNKNOW=-1
};

enum EUSER_DATA {
    EUSERDATA_SCINTILLA_TEXT_EDITOR = 1001
};

enum EROLE_DATA {
    EROLE_SUB_WIN       = Qt::UserRole+100,
    EROLE_FIND_POS

};

enum EHOT_KEYS {
    EHK_UNKNOW=-1
};

#define _CP_UTF16_LE        1200
#define _CP_UTF16_BE        1201

#define _NEW_FILE_PREFIX    "NewFile"
#define _BOOKMARK_NUM       1

#endif // CONSTANT_H
