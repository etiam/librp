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
Driver::parse(const std::string &filename)
{
    assert(!filename.empty());

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
    scanner->set_debug(m_debugLexer);
    parser->set_debug_level(m_debugParser);

    const int accept(0);
    if (parser->parse() != accept)
    {
        std::cerr << "Parse failed!!\n";
    }
}

void
Driver::debug(bool debugLexer, bool debugParser)
{
    m_debugLexer = debugLexer;
    m_debugParser = debugParser;
}

void
Driver::Attribute(Token name, int n, Token nms[], RtPointer vals[], int lengths[])
{
}

void
Driver::AttributeBegin()
{
}

void
Driver::AttributeEnd()
{
}

void
Driver::Begin(Token)
{
}

void
Driver::Color(RtColor)
{
}

void
Driver::CoordinateSystem(RtString)
{
}

void
Driver::ConcatTransform(RtMatrix)
{
}

void
Driver::CropWindow(float float1, float float2, float float3, float float4)
{
}

void
Driver::Declare(RtString, RtString)
{
}

void
Driver::Display(RtString, Token, Token, int int1, Token[], RtPointer[], int[])
{
}

void
Driver::Cylinder(float float1, float float2, float float3, float float4)
{
}

void
Driver::End()
{
}

void
Driver::Hider(RtString, int int1, Token[], RtPointer[], int[])
{
}

void
Driver::Format(int int1, int int2, float float1)
{
}

void
Driver::FrameAspectRatio(float float1)
{
}

void
Driver::LightSource(Token, int int1, Token[], RtPointer[], int[])
{
}

void
Driver::NuPatch(int int1, int int2, float[], float float1, float float2, int int3, int int4, float[], float float3, float float4, int int5, Token[], RtPointer[], int[])
{
}

void
Driver::Option(RtString, int int1, Token[], RtPointer[], int[])
{
}

void
Driver::Orientation(RtString)
{
}

void
Driver::PixelSamples(float float1, float float2)
{
}

void
Driver::PointsPolygons(int int1, int[], int[], int int2, Token[], RtPointer[], int[])
{
}

void
Driver::Polygon(int int1, int int2, Token[], RtPointer[], int[])
{
}

void
Driver::Projection(Token, int int1, Token[], RtPointer[], int[])
{
}

void
Driver::ReverseOrientation()
{
}

void
Driver::Rotate(float float1, float float2, float float3, float float4)
{
}

void
Driver::ShadingInterpolation(RtString)
{
}

void
Driver::Sphere(float float1, float float2, float float3, float float4)
{
}

void
Driver::Surface(Token, int int1, Token[], RtPointer[], int[])
{
}

void
Driver::TransformBegin()
{
}

void
Driver::TransformEnd()
{
}

void
Driver::Translate(float float1, float float2, float float3)
{
}

void
Driver::TrimCurve(int int1, int[], int[], float[], float[], float[], int[], float[], float[], float[])
{
}

void
Driver::WorldBegin()
{
}

void
Driver::WorldEnd()
{
}

}