#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>

#include "rp/driver.h"

int
main (int argc, char **argv)
{
    if (argc != 2)
        return (EXIT_FAILURE);
    Rp::Driver driver;
    driver.trace_parsing = true;
//    driver.trace_scanning = true;
    driver.parse(argv[1]);

    return 1;
}
