#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstdint>

typedef     int         RtInt;
typedef     float       RtFloat;
typedef     short       RtBoolean;
typedef     char *      RtString;
typedef     char *      RtToken;
typedef     void *      RtPointer;
typedef     RtFloat     RtColor[3];
typedef     RtFloat     RtPoint[3];
typedef     RtFloat     RtMatrix[4][4];
typedef     RtPointer   RtLightHandle;

#define     RtVoid      void
#define     RI_NULL     ((RtToken)0)

namespace Rp
{

class Scanner;
class Parser;

class Driver
{
  public:
   Driver() = default;

   virtual ~Driver();

   void             parse(const char *filename);

   RtVoid           RrAttributeV(RtToken, RtInt, RtToken[], RtPointer[], RtInt[]);
   RtVoid           RrAttributeBegin();
   RtVoid           RrAttributeEnd();
   RtVoid           RrBegin(RtToken);
   RtVoid           RrColor(RtColor);
   RtVoid           RrCoordinateSystem(RtString);
   RtVoid           RrConcatTransform(RtMatrix);
   RtVoid           RrCropWindow(RtFloat,RtFloat,RtFloat,RtFloat);
   RtToken          RrDeclare(char*,char*);
   RtVoid           RrDisplayV(RtString,RtToken,RtToken,RtInt,RtToken[], RtPointer[],RtInt[]);
   RtVoid           RrCylinder(RtFloat,RtFloat,RtFloat,RtFloat);
   RtVoid           RrEnd(void);
   RtVoid           RrHider(RtString, RtInt, RtToken[], RtPointer[], RtInt[]);
   RtVoid           RrFormat(RtInt,RtInt,RtFloat);
   RtVoid           RrFrameAspectRatio(RtFloat);
   RtLightHandle    RrLightSourceV(RtToken,RtInt,RtToken[],RtPointer[],RtInt[]);
   RtVoid           RrNuPatch(RtInt,RtInt,RtFloat[],RtFloat,RtFloat,RtInt,RtInt, RtFloat[],RtFloat,RtFloat,RtInt,RtToken[], RtPointer[],RtInt[]);
   RtVoid           RrOption(RtString, RtInt, RtToken[], RtPointer[], RtInt[]);
   RtVoid           RrOrientation(RtString);
   RtVoid           RrPixelSamples(RtFloat,RtFloat);
   RtVoid           RrPointsPolygonsV(RtInt,RtInt[],RtInt[],RtInt,RtToken[], RtPointer[],RtInt[]);
   RtVoid           RrPolygonV(RtInt,RtInt,RtToken[],RtPointer[],RtInt[]);
   RtVoid           RrProjectionV(RtToken,RtInt,RtToken[],RtPointer[],RtInt[]);
   RtVoid           RrReverseOrientation();
   RtVoid           RrRotate(RtFloat,RtFloat,RtFloat,RtFloat);
   RtVoid           RrShadingInterpolation(RtString);
   RtVoid           RrSphere(RtFloat,RtFloat,RtFloat,RtFloat);
   RtVoid           RrSurface(RtToken);
   RtVoid           RrSurfaceV(RtToken,RtInt,RtToken[],RtPointer[],RtInt[]);
   RtVoid           RrTransformBegin(void);
   RtVoid           RrTransformEnd(void);
   RtVoid           RrTranslate(RtFloat,RtFloat,RtFloat);
   RtVoid           RrTrimCurve(RtInt,RtInt[],RtInt[],RtFloat[],RtFloat[],RtFloat[], RtInt[],RtFloat[],RtFloat[],RtFloat[]);
   RtVoid           RrWorldBegin(void);
   RtVoid           RrWorldEnd(void);

   std::ostream&    print(std::ostream &stream);

   /// enable debug output in the flex scanner
   bool             trace_scanning;

   /// enable debug output in the bison parser
   bool             trace_parsing;

private:
   Parser  *        parser  = nullptr;
   Scanner *        scanner = nullptr;
};

}

#endif
