#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <string>
#include <cstdint>
#include <memory>

typedef     void *      RtPointer;
typedef     float       RtColor[3];
typedef     float       RtPoint[3];
typedef     float       RtMatrix[4][4];

namespace Rp
{

using Token = std::string;

class Scanner;
class Parser;

class Driver
{
  public:
    Driver();
    virtual ~Driver();

    virtual void    parse(const std::string &filename);
    virtual void    debug(bool debugLexer, bool debugParser);

    virtual void    Attribute(Token name, int n, Token nms[], RtPointer vals[], int lengths[]);
    virtual void    AttributeBegin();
    virtual void    AttributeEnd();
    virtual void    Begin(Token);
    virtual void    Color(RtColor);
    virtual void    CoordinateSystem(Token);
    virtual void    ConcatTransform(RtMatrix);
    virtual void    CropWindow(float, float, float, float);
    virtual void    Declare(Token, Token);
    virtual void    Display(Token, Token, Token, int, Token[], RtPointer[], int[]);
    virtual void    Camera(Token);
    virtual void    Cylinder(float, float, float, float);
    virtual void    End();
    virtual void    ErrorHandler(Token);
    virtual void    Exposure(float, float);
    virtual void    GeometricApproximation(Token type, float value);
    virtual void    Hider(Token, int, Token[], RtPointer[], int[]);
    virtual void    Format(int,int,float);
    virtual void    FrameAspectRatio(float);
    virtual void    LightSource(Token,int,Token[],RtPointer[],int[]);
    virtual void    NuPatch(int, int, float[], float, float, int, int, float[], float, float, int, Token[], RtPointer[], int[]);
    virtual void    Matte(bool onoff);
    virtual void    Option(Token, int, Token[], RtPointer[], int[]);
    virtual void    Orientation(Token);
    virtual void    PixelFilter(Token func, float xwidth, float ywidth);
    virtual void    PixelSamples(float, float);
    virtual void    PointsGeneralPolygons(int npolys, int nloops[], int nverts[], int verts[], int n, Token nms[], RtPointer vals[], int lengths[]);
    virtual void    PointsPolygons(int npolys, int nverts[], int verts[], int n, Token nms[], RtPointer vals[]);
    virtual void    Polygon(int, int, Token[], RtPointer[], int[]);
    virtual void    Projection(Token, int, Token[], RtPointer[], int[]);
    virtual void    Quantize(Token type, int one, int min, int max, float dither);
    virtual void    RelativeDetail(float rel);
    virtual void    ReverseOrientation();
    virtual void    Rotate(float angle, float dx, float dy, float dz);
    virtual void    Scale(float sx, float sy, float sz);
    virtual void    ShadingInterpolation(Token);
    virtual void    Sides(int n);
    virtual void    Sphere(float, float, float, float);
    virtual void    Surface(Token, int, Token[], RtPointer[], int[]);
    virtual void    TransformBegin();
    virtual void    TransformEnd();
    virtual void    Translate(float tx, float ty, float tz);
    virtual void    TrimCurve(int, int[], int[], float[], float[], float[], int[], float[], float[], float[]);
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
