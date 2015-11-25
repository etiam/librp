#include <cctype>
#include <cassert>
#include <fstream>

#include <ut/make_unique.h>

#include "scanner.h"
#include "parser.hh"
#include "driver.h"

namespace Rp
{

Driver::Driver()
{
}

Driver::~Driver()
{
}

void
Driver::parse(const std::string &filename)
{
    assert(!filename.empty());

    std::ifstream in_file(filename);
    if (!in_file.good())
        exit(EXIT_FAILURE);

    try
    {
        m_scanner = std::make_unique<Scanner>(&in_file);
    }
    catch (std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    try
    {
        m_parser = std::make_unique<Parser>(*m_scanner, (*this));
    }
    catch (std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    //    scanner->setParser(parser);
    m_scanner->set_debug(m_debugLexer);
    m_parser->set_debug_level(m_debugParser);

    const int accept(0);
    if (m_parser->parse() != accept)
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
Driver::CoordinateSystem(Token)
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
Driver::Declare(Token, Token)
{
}

void
Driver::Display(Token, Token, Token, int int1, Token[], RtPointer[], int[])
{
}

void
Driver::Camera(Token)
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
Driver::ErrorHandler(Token allocator)
{
}

void
Driver::Exposure(float, float)
{
}

void
Driver::GeometricApproximation(Token type, float value)
{
}

void
Driver::Hider(Token, int int1, Token[], RtPointer[], int[])
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
Driver::Matte(bool onoff)
{
}

void
Driver::Option(Token, int int1, Token[], RtPointer[], int[])
{
}

void
Driver::Orientation(Token)
{
}

void
Driver::PixelSamples(float float1, float float2)
{
}

void
Driver::PixelFilter(Token func, float float1, float float2)
{
}

void
Driver::PointsGeneralPolygons(int npolys, int nloops[], int nverts[], int verts[], int n, Token nms[], RtPointer vals[], int lengths[])
{
}

void
Driver::PointsPolygons(int npolys, int nverts[], int verts[], int n, Token nms[], RtPointer vals[])
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
Driver::Quantize(Token type, int one, int min, int max, float dither)
{
}

void
Driver::RelativeDetail(float rel)
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
Driver::Scale(float sx, float sy, float sz)
{
}

void
Driver::ShadingInterpolation(Token)
{
}

void
Driver::Sides(int n)
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
Driver::Translate(float tx, float ty, float tz)
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
