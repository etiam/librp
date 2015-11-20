#ifndef __YACC_H__
#define __YACC_H__

#include <string>

bool parse(const std::string &filename, bool debug=false);

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
