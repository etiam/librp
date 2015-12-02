#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <string>
#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <boost/any.hpp>

using RtBoolean         = short;
using RtInt             = int;
using RtFloat           = float;
using RtToken           = std::string;
using RtMatrix          = std::array<std::array<float, 4>, 4>;
using RtBasis           = std::array<std::array<float, 4>, 4>;
using RtColor           = std::array<float, 3>;
using RtPoint           = std::array<float, 3>;
using RtPointer         = boost::any;
using RtInts            = std::vector<int>;
using RtPointers        = std::vector<std::vector<boost::any>>;
using RtTokens          = std::vector<std::string>;
using RtLightHandle     = RtPointer;

namespace Rp
{

class Scanner;
class Parser;

class Driver
{
  public:
    Driver();
    virtual ~Driver();

    virtual void    parse(const std::string &filename = "");
    virtual void    debug(bool debugLexer, bool debugParser);

    virtual void    Attribute(RtToken name, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    AttributeBegin();
    virtual void    AttributeEnd();
    virtual void    Basis(RtBasis u, RtInt ustep, RtBasis v, RtInt vstep);
    virtual void    Begin(RtToken);
    virtual void    Color(RtColor);
    virtual void    CoordinateSystem(RtToken);
    virtual void    ConcatTransform(RtMatrix matrix);
    virtual void    CropWindow(RtFloat, RtFloat, RtFloat, RtFloat);
    virtual void    Declare(RtToken, RtToken);
    virtual void    Display(RtToken name, RtToken type, RtToken, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Camera(RtToken);
    virtual void    Clipping(RtFloat nearplane, RtFloat farplane);
    virtual void    Cylinder(RtFloat, RtFloat, RtFloat, RtFloat);
    virtual void    End();
    virtual void    ErrorHandler(RtToken);
    virtual void    Exposure(RtFloat gain, RtFloat gamma);
    virtual void    GeometricApproximation(RtToken type, RtFloat value);
    virtual void    Hider(RtToken, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Illuminate(RtLightHandle light, RtBoolean onoff);
    virtual void    Format(RtInt xres, RtInt yres, RtFloat pixel_ar);
    virtual void    FrameAspectRatio(RtFloat);
    virtual void    FrameBegin(RtInt frame);
    virtual void    FrameEnd();
    virtual void    LightSource(RtToken name, RtToken handle, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    NuPatch(RtInt, RtInt, RtFloat[], RtFloat, RtFloat, RtInt, RtInt, RtFloat[], RtFloat, RtFloat, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Matte(bool onoff);
    virtual void    Option(RtToken name, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Orientation(RtToken orient);
    virtual void    PixelFilter(RtToken func, RtFloat xwidth, RtFloat ywidth);
    virtual void    PixelSamples(RtFloat x, RtFloat y);
    virtual void    PointsGeneralPolygons(RtInt npolys, RtInts nloops, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    PointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Polygon(RtInt, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Projection(RtToken name, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Quantize(RtToken type, RtInt one, RtInt min, RtInt max, RtFloat dither);
    virtual void    RelativeDetail(RtFloat rel);
    virtual void    ReverseOrientation();
    virtual void    Rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual void    Scale(RtFloat sx, RtFloat sy, RtFloat sz);
    virtual void    ScreenWindow(RtFloat left, RtFloat right, RtFloat bottom, RtFloat top);
    virtual void    ShadingInterpolation(RtToken type);
    virtual void    ShadingRate(RtFloat size);
    virtual void    Sides(RtInt n);
    virtual void    Sphere(RtFloat, RtFloat, RtFloat, RtFloat);
    virtual void    Surface(RtToken, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    TransformBegin();
    virtual void    TransformEnd();
    virtual void    Translate(RtFloat tx, RtFloat ty, RtFloat tz);
    virtual void    TrimCurve(RtInt, RtInt[], RtInt[], RtFloat[], RtFloat[], RtFloat[], RtInt[], RtFloat[], RtFloat[], RtFloat[]);
    virtual void    WorldBegin();
    virtual void    WorldEnd();

  private:
    bool            m_debugLexer = false;
    bool            m_debugParser = false;

    std::unique_ptr<Parser>     m_parser;
    std::unique_ptr<Scanner>    m_scanner;
};

}

#endif
