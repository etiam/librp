#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "driver.h"

using namespace Rp;

class Catrib : public Driver
{
  public:
    virtual void            Attribute(std::string name, int n, std::string nms[], RtPointer vals[], int lengths[]);
    virtual void            ConcatTransform(RtMatrix matrix);
    virtual void            PointsGeneralPolygons(int npolys, int nloops[], int nverts[], int verts[], int n, std::string nms[], RtPointer vals[], int lengths[]);
    virtual void            Rotate(float angle, float dx, float dy, float dz);
    virtual void            TransformBegin();

  private:
    std::string             argListToString(int n, std::string nms[], RtPointer vals[], int lengths[]);

};

void
Catrib::Attribute(std::string name, int n, std::string nms[], RtPointer vals[], int lengths[])
{
    std::cout << "Attribute " << "\"" << name << "\"" << argListToString(n, nms, vals, lengths) << std::endl;
}

void
Catrib::ConcatTransform(RtMatrix matrix)
{
    std::cout << "ConcatTransform ";
    std::cout << "[";
    std::cout << matrix[0][0] << " " << matrix[0][1] << " " <<  matrix[0][2] << " " << matrix[0][3] << " "
              << matrix[1][0] << " " << matrix[1][1] << " " <<  matrix[1][2] << " " << matrix[0][3] << " "
              << matrix[2][0] << " " << matrix[2][1] << " " <<  matrix[2][2] << " " << matrix[0][3] << " "
              << matrix[3][0] << " " << matrix[3][1] << " " <<  matrix[3][2] << " " << matrix[0][3];
    std::cout << "]" << std::endl;
}

void
Catrib::PointsGeneralPolygons(int npolys, int nloops[], int nverts[], int verts[], int n, std::string nms[], RtPointer vals[], int lengths[])
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

std::string
Catrib::argListToString(int n, std::string nms[], RtPointer vals[], int lengths[])
{
    std::stringstream out;

    for (auto i = 0; i < n; ++i)
    {
        auto v = vals[i];

        out << " \"" << nms[i] << "\" [";

        if (nms[i].find("string") != std::string::npos)
            out << "\"" << static_cast<char*>(v) << "\"";
        else if (nms[i].find("int") != std::string::npos)
        {
            for (auto j=0; j < lengths[i] / sizeof(float); ++j)
            {
                out << static_cast<int>(static_cast<float*>(v)[j]) << " ";
            }
        }
        else if (nms[i].find("float") != std::string::npos ||
                 nms[i].find("P") != std::string::npos)
        {
            for (auto j=0; j < lengths[i] / sizeof(float); ++j)
            {
                out << static_cast<float*>(v)[j] << " ";
            }
        }
        out << "]";
    }
    return out.str();
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
