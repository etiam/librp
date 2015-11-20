#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <rp/parse.h>

int
main (int argc, char **argv)
{
    Rp::parseRib(argv[1]);
    return 1;
}
