#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <string>
#include <cstdint>

typedef     char *      RtString;
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
   Driver() = default;
   virtual ~Driver();

   virtual void             parse(const std::string &filename);
   virtual void             debug(bool debugLexer, bool debugParser);

   virtual void             Attribute(Token name, int n, Token nms[], RtPointer vals[], int lengths[]);
   virtual void             AttributeBegin();
   virtual void             AttributeEnd();
   virtual void             Begin(Token);
   virtual void             Color(RtColor);
   virtual void             CoordinateSystem(RtString);
   virtual void             ConcatTransform(RtMatrix);
   virtual void             CropWindow(float, float, float, float);
   virtual void             Declare(RtString, RtString);
   virtual void             Display(RtString, Token, Token, int, Token[], RtPointer[], int[]);
   virtual void             Cylinder(float, float, float, float);
   virtual void             End();
   virtual void             Hider(RtString, int, Token[], RtPointer[], int[]);
   virtual void             Format(int,int,float);
   virtual void             FrameAspectRatio(float);
   virtual void             LightSource(Token,int,Token[],RtPointer[],int[]);
   virtual void             NuPatch(int, int, float[], float, float, int, int, float[], float, float, int, Token[], RtPointer[], int[]);
   virtual void             Option(RtString, int, Token[], RtPointer[], int[]);
   virtual void             Orientation(RtString);
   virtual void             PixelSamples(float, float);
   virtual void             PointsPolygons(int, int[], int[], int, Token[], RtPointer[], int[]);
   virtual void             Polygon(int, int, Token[], RtPointer[], int[]);
   virtual void             Projection(Token, int, Token[], RtPointer[], int[]);
   virtual void             ReverseOrientation();
   virtual void             Rotate(float angle, float dx, float dy, float dz);
   virtual void             ShadingInterpolation(RtString);
   virtual void             Sphere(float, float, float, float);
   virtual void             Surface(Token, int, Token[], RtPointer[], int[]);
   virtual void             TransformBegin();
   virtual void             TransformEnd();
   virtual void             Translate(float, float, float);
   virtual void             TrimCurve(int, int[], int[], float[], float[], float[], int[], float[], float[], float[]);
   virtual void             WorldBegin();
   virtual void             WorldEnd();


private:
   bool                     m_debugLexer = false;
   bool                     m_debugParser = false;

   Parser  *                parser  = nullptr;
   Scanner *                scanner = nullptr;
};

}

#endif
