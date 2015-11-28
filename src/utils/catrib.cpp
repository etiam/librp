#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/any.hpp>

#include "driver.h"

using namespace Rp;

template <class T>
std::string
dumpVector(const std::vector<T> &vector)
{
    std::stringstream stream;
    std::for_each(std::begin(vector), std::end(vector),
                  [&stream](const typename std::vector<T>::value_type &n) { stream << n << " "; });

    return "[" + stream.str().substr(0, stream.str().size()-1) + "]";
}

template <class T>
std::string
dumpAnyVector(const std::vector<boost::any> &vector)
{
    std::stringstream stream;
    std::for_each(std::begin(vector), std::end(vector),
                  [&stream](const typename std::vector<boost::any>::value_type &n) { stream << boost::any_cast<T>(n) << " "; });

    return "[" + stream.str().substr(0, stream.str().size()-1) + "]";
}


class Catrib : public Driver
{
  public:
    virtual void    Attribute(RtToken name, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    AttributeBegin();
    virtual void    AttributeEnd();
    virtual void    Basis(RtBasis u, RtInt ustep, RtBasis v, RtInt vstep);
    virtual void    Clipping(RtFloat nearplane, RtFloat farplane);
    virtual void    ConcatTransform(RtMatrix matrix);
    virtual void    Display(RtToken name, RtToken type, RtToken, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Exposure(RtFloat gain, RtFloat gamma);
    virtual void    Format(RtInt xres, RtInt yres, RtFloat pixel_ar);
    virtual void    FrameBegin(RtInt frame);
    virtual void    FrameEnd();
    virtual void    Matte(bool onoff);
    virtual void    Orientation(RtToken orient);
    virtual void    PointsGeneralPolygons(RtInt npolys, RtInts nloops, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    PixelFilter(RtToken func, RtFloat xwidth, RtFloat ywidth);
    virtual void    PixelSamples(RtFloat x, RtFloat y);
    virtual void    Projection(RtToken, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    RelativeDetail(RtFloat rel);
    virtual void    Rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual void    Scale(RtFloat sx, RtFloat sy, RtFloat sz);
    virtual void    ScreenWindow(RtFloat left, RtFloat right, RtFloat bottom, RtFloat top);
    virtual void    ShadingInterpolation(RtToken type);
    virtual void    ShadingRate(RtFloat size);
    virtual void    Sides(RtInt n);
    virtual void    TransformBegin();
    virtual void    TransformEnd();
    virtual void    Translate(RtFloat tx, RtFloat ty, RtFloat tz);
    virtual void    WorldBegin();
    virtual void    WorldEnd();

  private:
    std::string     argListToString(int n, RtTokens nms, RtPointers vals);

};

void
Catrib::Attribute(RtToken name, int n, RtTokens nms, RtPointers vals)
{
    std::cout << "Attribute " << "\"" << name << "\" " << argListToString(n, nms, vals) << std::endl;
}

void
Catrib::AttributeBegin()
{
    std::cout << "AttributeBegin" << std::endl;
}

void
Catrib::AttributeEnd()
{
    std::cout << "AttributeEnd" << std::endl;
}

void
Catrib::Basis(RtBasis u, RtInt ustep, RtBasis v, RtInt vstep)
{
    std::cout << "Basis ";
    std::cout << "[";
    std::cout << u[0][0] << " " << u[0][1] << " " <<  u[0][2] << " " << u[0][3] << " "
              << u[1][0] << " " << u[1][1] << " " <<  u[1][2] << " " << u[0][3] << " "
              << u[2][0] << " " << u[2][1] << " " <<  u[2][2] << " " << u[0][3] << " "
              << u[3][0] << " " << u[3][1] << " " <<  u[3][2] << " " << u[0][3];
    std::cout << "]";
    std::cout << " " << ustep << " ";
    std::cout << "[";
    std::cout << v[0][0] << " " << v[0][1] << " " <<  v[0][2] << " " << v[0][3] << " "
              << v[1][0] << " " << v[1][1] << " " <<  v[1][2] << " " << v[0][3] << " "
              << v[2][0] << " " << v[2][1] << " " <<  v[2][2] << " " << v[0][3] << " "
              << v[3][0] << " " << v[3][1] << " " <<  v[3][2] << " " << v[0][3];
    std::cout << "]";
    std::cout << " " << vstep << " ";

    std::cout << std::endl;
}

void
Catrib::Clipping(RtFloat nearplane, RtFloat farplane)
{
    std::cout << "Clipping " << nearplane << " " << farplane << std::endl;
}

void
Catrib::ConcatTransform(RtMatrix matrix)
{
    std::cout << "ConcatTransform ";
    std::cout << "[";
    std::cout << matrix[0][0] << " " << matrix[0][1] << " " <<  matrix[0][2] << " " << matrix[0][3] << " "
              << matrix[1][0] << " " << matrix[1][1] << " " <<  matrix[1][2] << " " << matrix[1][3] << " "
              << matrix[2][0] << " " << matrix[2][1] << " " <<  matrix[2][2] << " " << matrix[2][3] << " "
              << matrix[3][0] << " " << matrix[3][1] << " " <<  matrix[3][2] << " " << matrix[3][3];
    std::cout << "]" << std::endl;
}

void
Catrib::Display(RtToken name, RtToken type, RtToken mode, RtInt n, RtTokens nms, RtPointers vals)
{
    std::cout << "Display \"" << name << "\" \"" << type << "\" \"" << mode << "\" ";
    std::cout << argListToString(n, nms, vals) << std::endl;
}

void
Catrib::Exposure(RtFloat gain, RtFloat gamma)
{
    std::cout << "Exposure " << gain << " " << gamma << std::endl;
}

void
Catrib::Format(RtInt xres, RtInt yres, RtFloat pixel_ar)
{
    std::cout << "Format " << xres << " " << yres << " " << pixel_ar << std::endl;
}

void
Catrib::FrameBegin(RtInt frame)
{
    std::cout << "FrameBegin " << frame << std::endl;
}

void
Catrib::FrameEnd()
{
    std::cout << "FrameEnd" << std::endl;
}

void
Catrib::Matte(bool onoff)
{
    std::cout << "Matte " << onoff << std::endl;
}

void
Catrib::Orientation(RtToken orient)
{
    std::cout << "Orientation \"" << orient << "\"" << std::endl;
}

void
Catrib::PixelSamples(RtFloat x, RtFloat y)
{
    std::cout << "PixelSamples " << x << " " << y << std::endl;
}

void
Catrib::Projection(RtToken name, RtInt n, RtTokens nms, RtPointers vals)
{
    std::cout << "Projection \"" << name << "\" ";
    std::cout << argListToString(n, nms, vals) << std::endl;
}

void
Catrib::RelativeDetail(RtFloat rel)
{
    std::cout << "RelativeDetail " << rel << std::endl;
}

void
Catrib::PointsGeneralPolygons(RtInt npolys, RtInts nloops, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals)
{
    std::cout << "PointsGeneralPolygons ";

    std::cout << dumpVector(nloops) << " ";
    std::cout << dumpVector(nverts) << " ";
    std::cout << dumpVector(verts) << " ";
    std::cout << argListToString(n, nms, vals) << std::endl;
}

void
Catrib::PixelFilter(RtToken func, RtFloat width, RtFloat height)
{
    std::cout << "PixelFilter \"" << func << "\" " << width << " " << height << std::endl;
}

void
Catrib::Rotate(float angle, float dx, float dy, float dz)
{
    std::cout << "Rotate " << angle << " " << dx << " " << dy << " " << dz << std::endl;
}

void
Catrib::Scale(RtFloat sx, RtFloat sy, RtFloat sz)
{
    std::cout << "Scale " << sx << " " << sy << " " << sz << std::endl;
}

void
Catrib::ScreenWindow(RtFloat left, RtFloat right, RtFloat bottom, RtFloat top)
{
    std::cout << "ScreenWindow " << left << " " << right << " " << bottom << " " << top << std::endl;
}

void
Catrib::ShadingInterpolation(RtToken type)
{
    std::cout << "ShadingInterpolation \"" << type << "\"" << std::endl;
}

void
Catrib::ShadingRate(RtFloat size)
{
    std::cout << "ShadingRate " << size << std::endl;
}

void
Catrib::Sides(RtInt n)
{
    std::cout << "Sides " << n << std::endl;
}

void
Catrib::TransformBegin()
{
    std::cout << "TransformBegin" << std::endl;
}

void
Catrib::TransformEnd()
{
    std::cout << "TransformEnd" << std::endl;
}

void
Catrib::Translate(RtFloat tx, RtFloat ty, RtFloat tz)
{
    std::cout << "Translate " << tx << " " << ty << " " << tz << std::endl;
}

void
Catrib::WorldBegin()
{
    std::cout << "WorldBegin" << std::endl;
}

void
Catrib::WorldEnd()
{
    std::cout << "WorldEnd" << std::endl;
}


std::string
Catrib::argListToString(int n, RtTokens nms, RtPointers vals)
{
    std::stringstream out;

    for (auto i = 0; i < n; ++i)
    {
        auto v = vals[i];

        out << "\"" << nms[i] << "\" ";

        if (nms[i].find("string") != std::string::npos)
        {
            out << "[\"" << boost::any_cast<std::string>(v[0]) << "\"]";
        }
        else if (nms[i].find("int") != std::string::npos)
        {
            out << dumpAnyVector<int>(v);
        }
        else if (nms[i].find("float") != std::string::npos ||
                 nms[i].find("P") != std::string::npos)
        {
            out << dumpAnyVector<float>(v);
        }

        out << " ";
    }
    return out.str();
}

int
main (int argc, char **argv)
{
    if (argc != 2)
        return (EXIT_FAILURE);

    std::cout << "##Renderman RIB" << std::endl;
    std::cout << "version 3.04" << std::endl;
    Catrib driver;
//    driver.debug(true, true);
    driver.parse(argv[1]);

    return 1;
}
