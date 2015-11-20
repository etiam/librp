#ifndef __YACC_H__
#define __YACC_H__

#include <string>

bool parse(const std::string &filename);

typedef struct strArgNode
{
 int            iType,iListCount,iListSize;
 char           *sLabel,*sValue;
 float          *dValue;
 strArgNode     *Next;
} ArgNode;

#endif
