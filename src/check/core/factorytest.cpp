/*
 * factorytest.cpp
 *
 *  Created on: Sep 14, 2008
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <math.h>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE FactoryTest
#include <boost/test/unit_test.hpp>
#include <ut/make_unique.h>

#include "core/global.h"
#include "core/classPointers.h"
#include "core/exception.h"
#include "core/nodeFactory.h"
#include "core/nodeManager.h"
#include "core/relationshipManager.h"
#include "core/node.h"
#include "core/standardAttributes.h"

const float TOLERANCE = 0.0001;

class Group : public Sg::Node
{
  public:
    Group() = default;
    ~Group() = default;

    virtual std::string     nodeType() const { return "group"; }
    virtual void            initializeNode() {}
};

class Camera : public Sg::Node
{
  public:
    Camera() = default;
    ~Camera() = default;

    virtual std::string     nodeType() const { return "camera"; }
    virtual void            initializeNode() {}
};

class FileMesh : public Sg::Node
{
  public:
    FileMesh(const std::string &filename) {}
    ~FileMesh() = default;

    virtual std::string     nodeType() const { return "filemesh"; }
    virtual void            initializeNode() {}
};

std::unique_ptr<Sg::Node>
createGroup(void)
{
    return std::make_unique<Group>();
}

std::unique_ptr<Sg::Node>
createCamera()
{
    return std::make_unique<Camera>();
}

std::unique_ptr<Sg::Node>
createFileMesh(std::string filename)
{
    return std::make_unique<FileMesh>(filename);
}

struct GlobalInit
{
    GlobalInit()
    {
        Sg::initialize();

        auto &nodefactory = Sg::nodeFactory();
        nodefactory.registerCreator("camera", createCamera);
        nodefactory.registerCreator("filemesh", createFileMesh);
        nodefactory.registerCreator("group", createGroup);

        Sg::registerRelationshipType(Sg::TypedRelationshipType<Group, Sg::Node>("contains"));
    }
};

BOOST_GLOBAL_FIXTURE( GlobalInit );

BOOST_AUTO_TEST_CASE ( NodeFactoryCreationTest )
{
    const auto camera = Sg::createNode("/interactive", "camera");
    const auto mesh = Sg::createNode<std::string>("/mesh", "filemesh", "foo.obj");

    BOOST_CHECK_EQUAL (Sg::findNode(camera->fullName()), camera);
    BOOST_CHECK_EQUAL (Sg::findNode(mesh->fullName()), mesh);

    BOOST_CHECK_EQUAL (camera->name(), "interactive");
    BOOST_CHECK_EQUAL (camera->fullName(), "/interactive");

    BOOST_CHECK_NO_THROW (Sg::findNode<Camera>(camera->fullName()));
    BOOST_CHECK (!Sg::findNode<Group>(camera->fullName()));

    BOOST_CHECK_EQUAL (Sg::nodes().size(), 2);

    BOOST_CHECK_EQUAL (Sg::nodes<Camera>().size(), 1);
    BOOST_CHECK_EQUAL (Sg::nodes<FileMesh>().size(), 1);

    BOOST_CHECK_EQUAL (Sg::nodes<Camera>()[0], camera);
    BOOST_CHECK_EQUAL (Sg::nodes<FileMesh>()[0], mesh);
}

BOOST_AUTO_TEST_CASE ( NodeFactoryDeletionTest )
{
    auto &relMgr = Sg::relationshipManager();

    const auto cu_camera = Sg::createNode("/closeup", "camera");
    std::string cu_camera_name = cu_camera->fullName();

    const auto zoom_camera = Sg::createNode("/zoom", "camera");
    const auto group = Sg::createNode("/grpa2", "group");

    Sg::makeTrue(group, "contains", cu_camera);
    Sg::makeTrue(group, "contains", zoom_camera);

    BOOST_CHECK_EQUAL (relMgr.listObjects(group, "contains")[0], cu_camera);
    BOOST_CHECK_EQUAL (relMgr.listObjects(group, "contains").size(), 2);

    Sg::deleteNode(cu_camera_name);

    BOOST_CHECK (!Sg::findNode(cu_camera_name));

    BOOST_CHECK_EQUAL (relMgr.listObjects(group, "contains").size(), 1);

    // must make explicit copy to prevent segfault when iterating over a list
    // that is being deleted
    auto nodes = Sg::nodes();
    for (const auto &node : nodes)
        Sg::deleteNode(node);

    BOOST_CHECK_EQUAL (Sg::nodes().size(), 0);
}

BOOST_AUTO_TEST_CASE ( AttrFactoryCreationTest )
{
    auto battr = Sg::asBoolAttribute(Sg::createAttribute("bool"));
    battr->set(true);
    BOOST_CHECK_EQUAL (battr->value(), true);
    BOOST_CHECK_EQUAL (battr->typeName(), "bool");

    auto sattr = Sg::asStringAttribute(Sg::createAttribute("string"));
    sattr->set("hello");
    BOOST_CHECK_EQUAL (sattr->value(), "hello");
    BOOST_CHECK_EQUAL (sattr->typeName(), "string");

    auto iattr = Sg::asIntAttribute(Sg::createAttribute("int"));
    iattr->set(64);
    BOOST_CHECK_EQUAL (iattr->value(), 64);
    BOOST_CHECK_EQUAL (iattr->typeName(), "int");

    auto fattr = Sg::asFloatAttribute(Sg::createAttribute("float"));
    fattr->set(M_PI);
    BOOST_CHECK_CLOSE (fattr->value(), M_PI, TOLERANCE);
    BOOST_CHECK_EQUAL (fattr->typeName(), "float");
}
