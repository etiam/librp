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
//Driver::Attribute(RtToken name, RtInt n, RtToken nms[], RtPointer vals[], RtInt lengths[])
Driver::Attribute(RtToken name, RtInt n, RtToken nms[], RtPointers vals, RtInt lengths[])
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
Driver::Basis(RtBasis u, RtInt ustep, RtBasis v, RtInt vstep)
{
}

void
Driver::Begin(RtToken)
{
}

void
Driver::Color(RtColor)
{
}

void
Driver::CoordinateSystem(RtToken)
{
}

void
Driver::ConcatTransform(RtMatrix matrix)
{
}

void
Driver::CropWindow(RtFloat RtFloat1, RtFloat RtFloat2, RtFloat RtFloat3, RtFloat RtFloat4)
{
}

void
Driver::Declare(RtToken, RtToken)
{
}

void
Driver::Display(RtToken, RtToken, RtToken, RtInt int1, RtToken[], RtPointer[], int[])
{
}

void
Driver::Camera(RtToken)
{
}

void
Driver::Cylinder(RtFloat RtFloat1, RtFloat RtFloat2, RtFloat RtFloat3, RtFloat RtFloat4)
{
}

void
Driver::End()
{
}

void
Driver::ErrorHandler(RtToken allocator)
{
}

void
Driver::Exposure(RtFloat, RtFloat)
{
}

void
Driver::GeometricApproximation(RtToken type, RtFloat value)
{
}

void
Driver::Hider(RtToken, RtInt int1, RtToken[], RtPointer[], int[])
{
}

void
Driver::Format(RtInt int1, RtInt int2, RtFloat RtFloat1)
{
}

void
Driver::FrameAspectRatio(RtFloat RtFloat1)
{
}

void
Driver::LightSource(RtToken, RtInt int1, RtToken[], RtPointer[], int[])
{
}

void
Driver::NuPatch(RtInt int1, RtInt int2, RtFloat[], RtFloat RtFloat1, RtFloat RtFloat2, RtInt int3, RtInt int4, RtFloat[], RtFloat RtFloat3, RtFloat RtFloat4, RtInt int5, RtToken[], RtPointer[], int[])
{
}

void
Driver::Matte(bool onoff)
{
}

void
Driver::Option(RtToken, RtInt int1, RtToken[], RtPointer[], int[])
{
}

void
Driver::Orientation(RtToken)
{
}

void
Driver::PixelSamples(RtFloat RtFloat1, RtFloat RtFloat2)
{
}

void
Driver::PixelFilter(RtToken func, RtFloat RtFloat1, RtFloat RtFloat2)
{
}

void
Driver::PointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtInt n, RtToken nms[], RtPointer vals[], RtInt lengths[])
{
}

void
Driver::PointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken nms[], RtPointer vals[])
{
}

void
Driver::Polygon(RtInt int1, RtInt int2, RtToken[], RtPointer[], int[])
{
}

void
Driver::Projection(RtToken, RtInt int1, RtToken[], RtPointer[], int[])
{
}

void
Driver::Quantize(RtToken type, RtInt one, RtInt min, RtInt max, RtFloat dither)
{
}

void
Driver::RelativeDetail(RtFloat rel)
{
}

void
Driver::ReverseOrientation()
{
}

void
Driver::Rotate(RtFloat RtFloat1, RtFloat RtFloat2, RtFloat RtFloat3, RtFloat RtFloat4)
{
}

void
Driver::Scale(RtFloat sx, RtFloat sy, RtFloat sz)
{
}

void
Driver::ShadingInterpolation(RtToken)
{
}

void
Driver::Sides(RtInt n)
{
}

void
Driver::Sphere(RtFloat RtFloat1, RtFloat RtFloat2, RtFloat RtFloat3, RtFloat RtFloat4)
{
}

void
Driver::Surface(RtToken, RtInt int1, RtToken[], RtPointer[], int[])
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
Driver::Translate(RtFloat tx, RtFloat ty, RtFloat tz)
{
}

void
Driver::TrimCurve(RtInt int1, int[], int[], RtFloat[], RtFloat[], RtFloat[], int[], RtFloat[], RtFloat[], RtFloat[])
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
