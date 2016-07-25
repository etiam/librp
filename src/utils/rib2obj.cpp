/*
 * ribImporter.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <ut/utils.h>
#include <ut/glmMath.h>
#include <ut/box.h>

#include "driver.h"

namespace
{

struct RibState
{
  unsigned int                  m_meshnum = 1;
  float                         m_fov = 90.0f;
  float                         m_aspect = 1.0f;
  float                         m_clipNear = 0.1f;
  float                         m_clipFar = 10000.0f;
  std::stack<glm::mat4>         m_transformStack;
  glm::mat4                     m_currentXform = glm::mat4();
  std::vector<std::string>      m_geomPaths = { "." };
  std::ofstream                 m_outstream;
};

using namespace Rp;

class RibImporter : public Driver
{
  public:
    RibImporter(RibState &state);
    virtual ~RibImporter();

    virtual void    Attribute(RtToken name, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    AttributeBegin();
    virtual void    AttributeEnd();
    virtual void    Basis(RtBasis u, RtInt ustep, RtBasis v, RtInt vstep);
    virtual void    Clipping(RtFloat nearplane, RtFloat farplane);
    virtual void    ConcatTransform(RtMatrix matrix);
    virtual void    Display(RtToken name, RtToken type, RtToken, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Exposure(RtFloat gain, RtFloat gamma);
    virtual void    Format(RtInt xres, RtInt yres, RtFloat pixel_ar);
    virtual void    FrameBegin(RtInt frame);
    virtual void    FrameEnd();
    virtual void    LightSource(RtToken name, RtInt handle, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Matte(bool onoff);
    virtual void    Option(RtToken name, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Orientation(RtToken orient);
    virtual void    PixelFilter(RtToken func, RtFloat xwidth, RtFloat ywidth);
    virtual void    PixelSamples(RtFloat x, RtFloat y);
    virtual void    PointsGeneralPolygons(RtInt npolys, RtInts nloops, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    PointsPolygons(RtInt npolys, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    Projection(RtToken, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    ReadArchive(RtToken name, RtArchiveCallback callback, RtInt n, RtTokens nms, RtPointers vals);
    virtual void    RelativeDetail(RtFloat rel);
    virtual void    Rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual void    Scale(RtFloat sx, RtFloat sy, RtFloat sz);
    virtual void    ScreenWindow(RtFloat left, RtFloat right, RtFloat bottom, RtFloat top);
    virtual void    ShadingInterpolation(RtToken type);
    virtual void    ShadingRate(RtFloat size);
    virtual void    Sides(RtInt n);
    virtual void    TransformBegin();
    virtual void    TransformEnd();
    virtual void    Translate(RtFloat tx, RtFloat ty, RtFloat tz);
    virtual void    WorldBegin();
    virtual void    WorldEnd();

  private:
    RibState  &                 m_state;
};

RibImporter::RibImporter(RibState &state) :
    m_state(state)
{
}

RibImporter::~RibImporter()
{
}

void
RibImporter::Attribute(RtToken name, int n, RtTokens nms, RtPointers vals)
{
    if (name == "identifier")
    {
        for (auto i = 0; i < n; ++i)
        {
            if (nms[i].find("string") != std::string::npos)
            {
            }
        }
    }
}

void
RibImporter::AttributeBegin()
{
}

void
RibImporter::AttributeEnd()
{
}

void
RibImporter::Basis(RtBasis u, RtInt ustep, RtBasis v, RtInt vstep)
{
}

void
RibImporter::Clipping(RtFloat nearplane, RtFloat farplane)
{
    m_state.m_clipNear = nearplane;
    m_state.m_clipFar = farplane;
}

void
RibImporter::ConcatTransform(RtMatrix matrix)
{
    auto mat = glm::mat4(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
                         matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
                         matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
                         matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);

    m_state.m_currentXform = m_state.m_currentXform * mat;
}

void
RibImporter::Display(RtToken name, RtToken type, RtToken mode, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
RibImporter::Exposure(RtFloat gain, RtFloat gamma)
{
}

void
RibImporter::Format(RtInt xres, RtInt yres, RtFloat pixel_ar)
{
    m_state.m_aspect = static_cast<float>(xres) / static_cast<float>(yres);
}

void
RibImporter::FrameBegin(RtInt frame)
{
}

void
RibImporter::FrameEnd()
{
}

void
RibImporter::LightSource(RtToken name, RtInt handle, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
RibImporter::Matte(bool onoff)
{
}

void
RibImporter::Option(RtToken name, RtInt n, RtTokens nms, RtPointers vals)
{
    // store '#Option "searchpath" "archive"' for later ReadArchive calls
    if (name == "searchpath")
    {
        for (unsigned int i=0; i < n; ++i)
        {
            if (nms[i] == "archive")
            {
                m_state.m_geomPaths = Ut::splitString(boost::any_cast<std::string>(vals[i][0]), ":");
            }
        }
    }
}

void
RibImporter::Orientation(RtToken orient)
{
}

void
RibImporter::PixelFilter(RtToken func, RtFloat width, RtFloat height)
{
}

void
RibImporter::PixelSamples(RtFloat x, RtFloat y)
{
}

void
RibImporter::PointsGeneralPolygons(RtInt npolys, RtInts nloops, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals)
{
    PointsPolygons(npolys, nverts, verts, n, nms, vals);
}

void
RibImporter::PointsPolygons(RtInt npolys, RtInts nverts, RtInts verts, RtInt n, RtTokens nms, RtPointers vals)
{
    auto &strm = m_state.m_outstream;
    strm << "g mesh" << m_state.m_meshnum << std::endl;

    // make rotation only xform for transforming normals
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_state.m_currentXform, scale, rotation, translation, skew, perspective);
    auto normalmat = glm::mat3(m_state.m_currentXform * glm::scale(glm::abs(glm::vec3(1.0)/scale)));
    (void)normalmat;

    // attributes state
    bool hasnormals = false;
    bool hastexcoords = false;

    // write attributes
    for (auto i = 0; i < n; ++i)
    {
        // positions
        if (nms[i] == "P")
        {
            std::cout << "lenP = " << vals[i].size() << std::endl;
            for (auto it=std::begin(vals[i]); it != std::end(vals[i]); it+=3)
            {
                auto pos = m_state.m_currentXform * glm::vec3(boost::any_cast<float>(*(it+0)), boost::any_cast<float>(*(it+1)), boost::any_cast<float>(*(it+2)));
                strm << "v "
                     << pos.x << " "
                     << pos.y << " "
                     << pos.z
                     << std::endl;
            }
        }

        // normals
        else if (nms[i] == "N")
        {
            std::cout << "lenN = " << vals[i].size() << std::endl;
            hasnormals = true;
            for (auto it=std::begin(vals[i]); it != std::end(vals[i]); it+=3)
            {
                auto nrm = normalmat * glm::vec3(boost::any_cast<float>(*(it+0)), boost::any_cast<float>(*(it+1)), boost::any_cast<float>(*(it+2)));
                strm << "vn "
                     << nrm.x << " "
                     << nrm.y << " "
                     << nrm.z
                     << std::endl;
            }
        }

        // texcoords
        else if (nms[i] == "st")
        {
            std::cout << "lenST = " << vals[i].size() << std::endl;
            hastexcoords = true;
            for (auto it=std::begin(vals[i]); it != std::end(vals[i]); it+=2)
            {
                strm << "vt "
                     << boost::any_cast<float>(*(it+0)) << " "
                     << boost::any_cast<float>(*(it+1))
                     << std::endl;
            }
        }
    }

    unsigned int s=0;
    for(auto valence : nverts)
    {
        strm << "f ";

        for (unsigned int e=0; e < valence; ++e)
        {
            strm << verts[s+e] + 1;
            if (hasnormals)
                strm << "/" << verts[s+e] + 1;
            if (hastexcoords)
                strm << "/" << verts[s+e] + 1;
            strm << " ";
        }

        s += valence;
        strm << std::endl;
    }

    m_state.m_meshnum++;

    /*
    auto vertcount = 0;
    for(auto count : nverts)
        vertcount += count;

    // init meshData
    auto meshData = std::make_unique<Core::MeshData>();
    meshData->m_hasPositions = true;
    meshData->m_hasNormals = false;
    meshData->m_hasTexCoords = false;
    meshData->m_hasVertColors = false;

    // reserve space for vertex/indice data
    meshData->m_positions.reserve(vertcount);
    meshData->m_indices.reserve(vertcount);

    // make one triangle chunk
    Core::MeshData::Chunk chunk;
    chunk.m_numIndices = vertcount;
    chunk.m_baseVertex = 0;
    chunk.m_baseIndex = 0;
    chunk.m_primitiveType = Core::MeshData::PrimitiveType::TRIANGLES;
    chunk.m_chunkType = Core::MeshData::ChunkType::ALWAYS;
    meshData->m_chunks.push_back(chunk);

    // Get P
    std::vector<float> P;
    for (auto i = 0; i < n; ++i)
    {
        if (nms[i].find("P") != std::string::npos)
        {
            std::for_each(std::begin(vals[i]), std::end(vals[i]),
                    [&P](const typename std::vector<boost::any>::value_type &n) { P.push_back(boost::any_cast<float>(n)); });
            break;
        }
    }
//    assert(P.size() / 3 == vertcount);

    // Get N
    std::vector<float> N;
    for (auto i = 0; i < n; ++i)
    {
        if (nms[i].find("N") != std::string::npos)
        {
            std::for_each(std::begin(vals[i]), std::end(vals[i]),
                    [&N](const typename std::vector<boost::any>::value_type &n) { N.push_back(boost::any_cast<float>(n)); });
            break;
        }
    }
    if (!N.empty())
    {
        meshData->m_hasNormals = true;
    }

    // Get ST
    std::vector<float> ST;
    for (auto i = 0; i < n; ++i)
    {
        if (nms[i].find("bakest") != std::string::npos)
        {
            std::for_each(std::begin(vals[i]), std::end(vals[i]),
                    [&ST](const typename std::vector<boost::any>::value_type &n) { ST.push_back(boost::any_cast<float>(n)); });
            break;
        }
    }
    if (!ST.empty())
    {
        meshData->m_hasTexCoords = true;
    }

    Ut::Box3f boundingBox;

    // make rotation only xform for transforming normals
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_state.m_currentXform, scale, rotation, translation, skew, perspective);
    auto normalmat = glm::mat3(m_state.m_currentXform * glm::scale(glm::abs(glm::vec3(1.0)/scale)));

    // store verts/normals/st
    for (auto i=0; i < vertcount; i++)
    {
        // indices
        auto ind = verts[i];
        meshData->m_indices.push_back(i);

        // position
        auto pos =  m_state.m_currentXform * glm::vec3(P[ind*3+0], P[ind*3+1], P[ind*3+2]);
        meshData->m_positions.push_back(glm::vec4(pos.xyz(), 0.0));

        // normals
        if (!N.empty())
        {
            auto nrm = normalmat * glm::vec3(N[ind*3+0], N[ind*3+1], N[ind*3+2]);
            meshData->m_normals.push_back(nrm);
        }

        // texcoords
        if (!ST.empty())
        {
            auto crd = glm::vec2(ST[ind*2+0], ST[ind*2+1]);
            meshData->m_texCoords.push_back(crd);
        }

        // use first vertex position as initial bbox value
        if (meshData->m_positions.size() == 1)
        {
            boundingBox.setMin(glm::vec3(pos.x, pos.y, pos.z));
            boundingBox.setMax(glm::vec3(pos.x, pos.y, pos.z));
        }

        // update bbox
        auto min = boundingBox.min();
        auto max = boundingBox.max();

        min.x = std::min(min.x, pos.x);
        min.y = std::min(min.y, pos.y);
        min.z = std::min(min.z, pos.z);

        max.x = std::max(max.x, pos.x);
        max.y = std::max(max.y, pos.y);
        max.z = std::max(max.z, pos.z);

        boundingBox.setMin(min);
        boundingBox.setMax(max);
    }

    auto meshnode = Core::asMeshObject(Sg::createNode(m_meshname, "mesh"));
    meshnode->setMeshData(std::move(meshData));
    meshnode->setLocalBoundingBox(boundingBox);
    */
}

void
RibImporter::Projection(RtToken name, RtInt n, RtTokens nms, RtPointers vals)
{
}

void
RibImporter::ReadArchive(RtToken name, RtArchiveCallback callback, RtInt n, RtTokens nms, RtPointers vals)
{
    bool success = false;

    namespace fs = boost::filesystem;

    // try to open the specified rib file as an absolute path
    if (fs::exists(name))
    {
        RibImporter driver(m_state);
        driver.parse(name);
        success = true;
    }
    else
    {
        // try to find/open the specified rib file in the geom path
        for (const auto &path : m_state.m_geomPaths)
        {
            fs::path filename(path);
            filename /= name;
            if (fs::exists(filename))
            {
                RibImporter driver(m_state);
                driver.parse(filename.string());
                success = true;
                break;
            }
        }
    }

    if (!success)
    {
        std::cout << "couldn't find \"" << name << "\" in geompath" << std::endl;
    }
}

void
RibImporter::RelativeDetail(RtFloat rel)
{
}

void
RibImporter::Rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
    m_state.m_currentXform = glm::rotate(glm::radians(angle), glm::vec3(dx, dy, dz)) * m_state.m_currentXform;
}

void
RibImporter::Scale(RtFloat sx, RtFloat sy, RtFloat sz)
{
    m_state.m_currentXform = glm::scale(glm::vec3(sx, sy, sz)) * m_state.m_currentXform;
}

void
RibImporter::ScreenWindow(RtFloat left, RtFloat right, RtFloat bottom, RtFloat top)
{
    auto width = right - left;
    auto height = top - bottom;

    if (width > height)
        m_state.m_fov = glm::degrees(std::atan(width / 2.0f)) * 2.0f;
    else
        m_state.m_fov = glm::degrees(std::atan(height / 2.0f)) * 2.0f;
}

void
RibImporter::ShadingInterpolation(RtToken type)
{
}

void
RibImporter::ShadingRate(RtFloat size)
{
}

void
RibImporter::Sides(RtInt n)
{
}

void
RibImporter::TransformBegin()
{
    m_state.m_transformStack.push(m_state.m_currentXform);
}

void
RibImporter::TransformEnd()
{
    assert(!m_state.m_transformStack.empty());
    m_state.m_currentXform = m_state.m_transformStack.top();
    m_state.m_transformStack.pop();
}

void
RibImporter::Translate(RtFloat tx, RtFloat ty, RtFloat tz)
{
    m_state.m_currentXform = glm::translate(glm::vec3(tx, ty, tz)) * m_state.m_currentXform;
}

void
RibImporter::WorldBegin()
{
    m_state.m_currentXform = glm::mat4();
}

void
RibImporter::WorldEnd()
{
}

}

int
main (int argc, char **argv)
{
    using namespace boost::program_options;

    options_description generic("Generic options");
    generic.add_options()
        ("in,i", value<std::string>(), "input file")
        ("out,o", value<std::string>(), "output file");

    options_description cmdline_options;
    cmdline_options.add(generic);

    options_description visible("Allowed options");
    visible.add(generic);

    variables_map vm;
    store(command_line_parser(argc, argv).options(cmdline_options).run(), vm);
    notify(vm);

    // --help
    if (vm.count("help") || (vm.count("in") == 0 || vm.count("out") == 0))
    {
        std::cout << "Usage: rib2obj" << std::endl << std::endl;
        std::cout << generic << std::endl;
        std::exit(0);
    }

    if (vm.count("in") && vm.count("out"))
    {
        RibState state;

        state.m_outstream.open(vm["out"].as<std::string>());

        if (state.m_outstream.good())
        {
            RibImporter driver(state);

            //    driver.debug(true, true);

            driver.parse(vm["in"].as<std::string>());

            state.m_outstream.close();
        }
    }

    return 1;
}
