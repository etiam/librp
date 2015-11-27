#ifndef __YACC_H__
#define __YACC_H__

#include <string>

typedef struct strArgNode
{
    int             iType;
    int             iListCount;
    int             iListSize;
    char *          sLabel;
    char *          sValue;
    float *         dValue;
    strArgNode *    Next;
} ArgNode;

#endif
