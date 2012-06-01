#ifndef MEM_H
#define MEM_H
#include "debug.h"

#define _DEL_MEM(e) \
    if (e) { delete e; e=NULL; }

#define _DEL_ARRAY(e) \
    if (e) { delete [] e; e=NULL; }


#endif // MEM_H
