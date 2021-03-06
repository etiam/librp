//#include <cctype>
//#include <cassert>



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
    m_scanner = std::make_unique<Scanner>(filename);
    m_parser = std::make_unique<Parser>(*m_scanner, (*this));
    m_parser->set_debug_level(m_debugParser);

    m_scanner->setParser(*m_parser);
    m_scanner->set_debug(m_debugLexer);

    if (m_parser->parse() != 0)
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

std::string
Driver::filename() const
{
    return m_scanner->filename();
}

void
Driver::Attribute(RtToken name, RtInt n, RtTokens nms, RtPointers vals)
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
Driver::Display(RtToken name, RtToken type, RtToken mode, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::DisplayChannel(RtToken channel, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::Camera(RtToken)
{
}

void
Driver::Clipping(RtFloat nearplane, RtFloat farplane)
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
Driver::Exposure(RtFloat gain, RtFloat gamma)
{
}

void
Driver::GeometricApproximation(RtToken type, RtFloat value)
{
}

void
Driver::Hider(RtToken, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::Illuminate(RtLightHandle light, RtBoolean onoff)
{
}

void
Driver::Format(RtInt xres, RtInt yres, RtFloat pixel_ar)
{
}

void
Driver::FrameBegin(RtInt frame)
{
}

void
Driver::FrameEnd()
{
}

void
Driver::FrameAspectRatio(RtFloat RtFloat1)
{
}

void
Driver::LightSource(RtToken name, RtInt handle, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::NuPatch(RtInt int1, RtInt int2, RtFloat[], RtFloat RtFloat1, RtFloat RtFloat2, RtInt int3, RtInt int4, RtFloat[], RtFloat RtFloat3, RtFloat RtFloat4, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::Matte(bool onoff)
{
}

void
Driver::Option(RtToken name, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::Orientation(RtToken orient)
{
}

void
Driver::PixelSamples(RtFloat x, RtFloat y)
{
}

void
Driver::PixelFilter(RtToken func, RtFloat width, RtFloat height)
{
}

void
Driver::PointsGeneralPolygons(RtInt npolys, RtInts nloops, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::PointsPolygons(RtInt npolys, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::Polygon(RtInt int1, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::Projection(RtToken name, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
Driver::Quantize(RtToken type, RtInt one, RtInt min, RtInt max, RtFloat dither)
{
}

void
Driver::ReadArchive(RtToken name, RtArchiveCallback callback, RtInt n, RtTokens nms, RtPointers vals)
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
Driver::ScreenWindow(RtFloat left, RtFloat right, RtFloat bottom, RtFloat top)
{
}

void
Driver::ShadingInterpolation(RtToken type)
{
}

void
Driver::ShadingRate(RtFloat size)
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
Driver::Surface(RtToken, RtInt n, RtTokens nms, RtPointers vals)
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
