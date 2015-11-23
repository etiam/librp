#ifndef __RR_H__
#define __RR_H__

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

RtVoid          RrAttribute(RtToken,RtInt,RtToken[],RtPointer[],RtInt[]);
RtVoid          RrAttributeBegin();
RtVoid          RrAttributeEnd();
RtVoid          RrBegin(RtToken);
RtVoid          RrColor(RtColor);
RtVoid          RrConcatTransform(RtMatrix);
RtVoid          RrCropWindow(RtFloat,RtFloat,RtFloat,RtFloat);
RtToken         RrDeclare(char*,char*);
RtVoid          RrDisplayV(RtString,RtToken,RtToken,RtInt,RtToken[], RtPointer[],RtInt[]);
RtVoid          RrCylinder(RtFloat,RtFloat,RtFloat,RtFloat);
RtVoid          RrEnd(void);
RtVoid          RrFormat(RtInt,RtInt,RtFloat);
RtVoid          RrFrameAspectRatio(RtFloat);
RtLightHandle   RrLightSourceV(RtToken,RtInt,RtToken[],RtPointer[],RtInt[]);
RtVoid          RrNuPatch(RtInt,RtInt,RtFloat[],RtFloat,RtFloat,RtInt,RtInt, RtFloat[],RtFloat,RtFloat,RtInt,RtToken[], RtPointer[],RtInt[]);
RtVoid          RrPixelSamples(RtFloat,RtFloat);
RtVoid          RrPointsPolygonsV(RtInt,RtInt[],RtInt[],RtInt,RtToken[], RtPointer[],RtInt[]);
RtVoid          RrPolygonV(RtInt,RtInt,RtToken[],RtPointer[],RtInt[]);
RtVoid          RrProjectionV(RtToken,RtInt,RtToken[],RtPointer[],RtInt[]);
RtVoid          RrReverseOrientation();
RtVoid          RrRotate(RtFloat,RtFloat,RtFloat,RtFloat);
RtVoid          RrSphere(RtFloat,RtFloat,RtFloat,RtFloat);
RtVoid          RrSurface(RtToken);
RtVoid          RrSurfaceV(RtToken,RtInt,RtToken[],RtPointer[],RtInt[]);
RtVoid          RrTransformBegin(void);
RtVoid          RrTransformEnd(void);
RtVoid          RrTranslate(RtFloat,RtFloat,RtFloat);
RtVoid          RrTrimCurve(RtInt,RtInt[],RtInt[],RtFloat[],RtFloat[],RtFloat[], RtInt[],RtFloat[],RtFloat[],RtFloat[]);
RtVoid          RrWorldBegin(void);
RtVoid          RrWorldEnd(void);

#endif

