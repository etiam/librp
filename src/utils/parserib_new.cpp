#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <iostream>

#include "rp/driver.h"

class Catrib : public Rp::Driver
{
  public:
   virtual void             TransformBegin();
};

void
Catrib::TransformBegin()
{
    std::cout << "TransformBegin" << std::endl;
}

int
main (int argc, char **argv)
{
    if (argc != 2)
        return (EXIT_FAILURE);

    Catrib driver;
//    driver.debug(true, true);
    driver.parse(argv[1]);

    return 1;
}
