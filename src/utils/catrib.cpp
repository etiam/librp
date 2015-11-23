#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <iostream>

#include "rp/driver.h"

using namespace Rp;

class Catrib : public Driver
{
  public:
   virtual void             Attribute(Token name, int n, Token nms[], RtPointer vals[], int lengths[]);
   virtual void             Rotate(float angle, float dx, float dy, float dz);
   virtual void             TransformBegin();
};

void
Catrib::Attribute(Token name, int n, Token nms[], RtPointer vals[], int lengths[])
{
    std::cout << "Attribute " << "\"" << name << "\"";
    std::cout << std::endl;
}

void
Catrib::Rotate(float angle, float dx, float dy, float dz)
{
    std::cout << "Rotate " << angle << " " << dx << " " << dy << " " << dz << std::endl;
}

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
