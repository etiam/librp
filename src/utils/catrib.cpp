#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <iostream>

#include "driver.h"

using namespace Rp;

class Catrib : public Driver
{
  public:
    virtual void            Attribute(Token name, int n, Token nms[], RtPointer vals[], int lengths[]);
    virtual void            PointsGeneralPolygons(int npolys, int nloops[], int nverts[], int verts[], int n, Token nms[], RtPointer vals[], int lengths[]);
    virtual void            Rotate(float angle, float dx, float dy, float dz);
    virtual void            TransformBegin();
};

void
Catrib::Attribute(Token name, int n, Token nms[], RtPointer vals[], int lengths[])
{
    std::cout << "Attribute " << "\"" << name << "\"" << argListToString(n, nms, vals, lengths) << std::endl;
}

void
Catrib::PointsGeneralPolygons(int npolys, int nloops[], int nverts[], int verts[], int n, Token nms[], RtPointer vals[], int lengths[])
{
    std::cout << "PointsGeneralPolygons ";

    {
    std::cout << "[";
    for (auto i=0; i < npolys; i++)
        std::cout << nloops[i] << " ";
    std::cout << "] ";
    }

    auto nverts_size = 0;
    {
    for(auto i=0; i < npolys; i++)
        nverts_size += nloops[i];
    }

    {
    std::cout << "[";
    for (auto i=0; i < nverts_size; i++)
        std::cout << nverts[i] << " ";
    std::cout << "] ";
    }

    auto verts_size = 0;
    {
    for(auto i=0; i < nverts_size; i++)
        verts_size += nverts[i];
    }

    {
    std::cout << "[";
    for (auto i=0; i < verts_size; i++)
        std::cout << verts[i] << " ";
    std::cout << "] ";
    }

    std::cout << argListToString(n, nms, vals, lengths) << std::endl;
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
