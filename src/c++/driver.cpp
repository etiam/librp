#include <cctype>
#include <fstream>
#include <cassert>

#include "scanner.h"
#include "parser.hh"
#include "driver.h"

namespace Rp
{

Driver::~Driver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void
Driver::parse(const char *const filename)
{
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good())
        exit(EXIT_FAILURE);

    delete (scanner);
    try
    {
        scanner = new Scanner(&in_file);
    }
    catch (std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    delete (parser);
    try
    {
        parser = new Parser((*scanner), (*this));
    }
    catch (std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    scanner->set_debug(trace_scanning);
    parser->set_debug_level(trace_parsing);

    const int accept(0);
    if (parser->parse() != accept)
    {
        std::cerr << "Parse failed!!\n";
    }
}

void
Driver::RrAttributeV(RtToken, RtInt int1, RtToken[], RtPointer[], RtInt[])
{
}

void
Driver::RrAttributeBegin()
{
}

void
Driver::RrAttributeEnd()
{
}

void
Driver::RrBegin(RtToken)
{
}

void
Driver::RrColor(RtColor)
{
}

void
Driver::RrCoordinateSystem(RtString)
{
}

void
Driver::RrConcatTransform(RtMatrix)
{
}

void
Driver::RrCropWindow(RtFloat float1, RtFloat float2, RtFloat float3, RtFloat float4)
{
}

RtToken
Driver::RrDeclare(char *, char *)
{
    return nullptr;
}

void
Driver::RrDisplayV(RtString, RtToken, RtToken, RtInt int1, RtToken[], RtPointer[], RtInt[])
{
}

void
Driver::RrCylinder(RtFloat float1, RtFloat float2, RtFloat float3, RtFloat float4)
{
}

void
Driver::RrEnd(void)
{
}

void
Driver::RrHider(RtString, RtInt int1, RtToken[], RtPointer[], RtInt[])
{
}

void
Driver::RrFormat(RtInt int1, RtInt int2, RtFloat float1)
{
}

void
Driver::RrFrameAspectRatio(RtFloat float1)
{
}

RtLightHandle
Driver::RrLightSourceV(RtToken, RtInt int1, RtToken[], RtPointer[], RtInt[])
{
    return nullptr;
}

void
Driver::RrNuPatch(RtInt int1,
    RtInt int2,
    RtFloat[],
    RtFloat float1,
    RtFloat float2,
    RtInt int3,
    RtInt int4,
    RtFloat[],
    RtFloat float3,
    RtFloat float4,
    RtInt int5,
    RtToken[],
    RtPointer[],
    RtInt[])
{
}

void
Driver::RrOption(RtString, RtInt int1, RtToken[], RtPointer[], RtInt[])
{
}

void
Driver::RrOrientation(RtString)
{
}

void
Driver::RrPixelSamples(RtFloat float1, RtFloat float2)
{
}

void
Driver::RrPointsPolygonsV(RtInt int1, RtInt[], RtInt[], RtInt int2, RtToken[], RtPointer[], RtInt[])
{
}

void
Driver::RrPolygonV(RtInt int1, RtInt int2, RtToken[], RtPointer[], RtInt[])
{
}

void
Driver::RrProjectionV(RtToken, RtInt int1, RtToken[], RtPointer[], RtInt[])
{
}

void
Driver::RrReverseOrientation()
{
}

void
Driver::RrRotate(RtFloat float1, RtFloat float2, RtFloat float3, RtFloat float4)
{
}

void
Driver::RrShadingInterpolation(RtString)
{
}

void
Driver::RrSphere(RtFloat float1, RtFloat float2, RtFloat float3, RtFloat float4)
{
}

void Driver::RrSurface(RtToken)
{
}

void
Driver::RrSurfaceV(RtToken, RtInt int1, RtToken[], RtPointer[], RtInt[])
{
}

void
Driver::RrTransformBegin(void)
{
}

void
Driver::RrTransformEnd(void)
{
}

void
Driver::RrTranslate(RtFloat float1, RtFloat float2, RtFloat float3)
{
}

void
Driver::RrTrimCurve(RtInt int1,
    RtInt[],
    RtInt[],
    RtFloat[],
    RtFloat[],
    RtFloat[],
    RtInt[],
    RtFloat[],
    RtFloat[],
    RtFloat[])
{
}

void
Driver::RrWorldBegin(void)
{
}

void
Driver::RrWorldEnd(void)
{
}

std::ostream &
Driver::print(std::ostream &stream)
{
   return(stream);
}

}
