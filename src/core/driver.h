#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <string>
#include <cstdint>
#include <memory>
#include <array>

using RtPointer = void *;
using RtInt     = int;
using RtFloat   = float;
using RtToken   = std::string;
using RtMatrix  = std::array<std::array<float, 4>, 4>;
using RtColor   = std::array<float, 3>;
using RtPoint   = std::array<float, 3>;

namespace Rp
{

class Scanner;
class Parser;

class Driver
{
  public:
    Driver();
    virtual ~Driver();

    virtual void    parse(const RtToken &filename);
    virtual void    debug(bool debugLexer, bool debugParser);

    virtual void    Attribute(RtToken name, RtInt n, RtToken nms[], RtPointer vals[], RtInt lengths[]);
    virtual void    AttributeBegin();
    virtual void    AttributeEnd();
    virtual void    Begin(RtToken);
    virtual void    Color(RtColor);
    virtual void    CoordinateSystem(RtToken);
    virtual void    ConcatTransform(RtMatrix matrix);
    virtual void    CropWindow(RtFloat, RtFloat, RtFloat, RtFloat);
    virtual void    Declare(RtToken, RtToken);
    virtual void    Display(RtToken, RtToken, RtToken, RtInt, RtToken[], RtPointer[], RtInt[]);
    virtual void    Camera(RtToken);
    virtual void    Cylinder(RtFloat, RtFloat, RtFloat, RtFloat);
    virtual void    End();
    virtual void    ErrorHandler(RtToken);
    virtual void    Exposure(RtFloat, RtFloat);
    virtual void    GeometricApproximation(RtToken type, RtFloat value);
    virtual void    Hider(RtToken, RtInt, RtToken[], RtPointer[], RtInt[]);
    virtual void    Format(RtInt, RtInt, RtFloat);
    virtual void    FrameAspectRatio(RtFloat);
    virtual void    LightSource(RtToken, RtInt, RtToken[], RtPointer[], RtInt[]);
    virtual void    NuPatch(RtInt, RtInt, RtFloat[], RtFloat, RtFloat, RtInt, RtInt, RtFloat[], RtFloat, RtFloat, RtInt, RtToken[], RtPointer[], RtInt[]);
    virtual void    Matte(bool onoff);
    virtual void    Option(RtToken, RtInt, RtToken[], RtPointer[], RtInt[]);
    virtual void    Orientation(RtToken);
    virtual void    PixelFilter(RtToken func, RtFloat xwidth, RtFloat ywidth);
    virtual void    PixelSamples(RtFloat, RtFloat);
    virtual void    PointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtInt n, RtToken nms[], RtPointer vals[], RtInt lengths[]);
    virtual void    PointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken nms[], RtPointer vals[]);
    virtual void    Polygon(RtInt, RtInt, RtToken[], RtPointer[], RtInt[]);
    virtual void    Projection(RtToken, RtInt, RtToken[], RtPointer[], RtInt[]);
    virtual void    Quantize(RtToken type, RtInt one, RtInt min, RtInt max, RtFloat dither);
    virtual void    RelativeDetail(RtFloat rel);
    virtual void    ReverseOrientation();
    virtual void    Rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual void    Scale(RtFloat sx, RtFloat sy, RtFloat sz);
    virtual void    ShadingInterpolation(RtToken);
    virtual void    Sides(RtInt n);
    virtual void    Sphere(RtFloat, RtFloat, RtFloat, RtFloat);
    virtual void    Surface(RtToken, RtInt, RtToken[], RtPointer[], RtInt[]);
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
