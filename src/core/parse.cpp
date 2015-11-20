#include "yacc.h"
#include "parse.h"

namespace Rp
{

void
parseRib(const std::string &filename)
{
    parse(filename);
}

}
