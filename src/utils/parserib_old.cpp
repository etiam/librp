#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>

#include "rp/parse.h"

int
main (int argc, char **argv)
{
    if (argc != 2)
        return (EXIT_FAILURE);
    Rp::parseRib(argv[1]);

    return 1;
}
