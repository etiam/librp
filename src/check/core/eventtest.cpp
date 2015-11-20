/*
 * factorytest.cpp
 *
 *  Created on: Sep 14, 2008
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EventTest
#include <boost/test/unit_test.hpp>

#include "core/global.h"
#include "core/classPointers.h"
#include "core/exception.h"
#include "core/node.h"
#include "core/floatAttribute.h"

std::string createdoidname = "";
std::string deletedoidname = "";

std::string createdattrname = "";
std::string changedattrname = "";
std::string deletedattrname = "";

std::string relationaddedsubject;
std::string relationaddedpredicate = "";
std::string relationaddedobject;
std::string relationremovedsubject;
std::string relationremovedpredicate = "";
std::string relationremovedobject;

class Camera : public Sg::Node
{
  public:
    Camera() = default;
    ~Camera() = default;

    float aperature() const { return 12.0f; }

    virtual std::string     nodeType() const { return "camera"; }
    virtual void            initializeNode() {}
};

std::unique_ptr<Sg::Node>
createCamera()
{
    return std::make_unique<Camera>();
}

class Group : public Sg::Node
{
  public:
    Group() = default;
    ~Group() = default;

    virtual std::string     nodeType() const { return "group"; }
    virtual void            initializeNode() {}
};

std::unique_ptr<Sg::Node>
createGroup(void)
{
    return std::make_unique<Group>();
}

void
onNodesCreated(const Sg::EventData<Sg::NodePtr> &events)
{
    createdoidname = std::get<0>(events[0])->name();
}

void
onNodesDeleted(const Sg::EventData<std::string> &nodeNames)
{
    deletedoidname = std::get<0>(nodeNames[0]);
}

void
onAttrsCreated(const Sg::EventData<Sg::NodePtr, Sg::AttributePtr> &attrs)
{
    std::stringstream fullattrname;
    fullattrname << std::get<0>(attrs[0])->fullName() << ":" << std::get<1>(attrs[0])->name();
    createdattrname = fullattrname.str();
}

void
onAttrsChanged(const Sg::EventData<Sg::NodePtr, Sg::AttributePtr> &attrs)
{
    std::stringstream fullattrname;
    fullattrname << std::get<0>(attrs[0])->fullName() << ":" << std::get<1>(attrs[0])->name();
    changedattrname = fullattrname.str();
}

void
onAttrsDeleted(const Sg::EventData<Sg::NodePtr, std::string> &attrs)
{
    std::stringstream fullattrname;
    fullattrname << std::get<0>(attrs[0])->fullName() << ":" << std::get<1>(attrs[0]);
    deletedattrname = fullattrname.str();
}

void
onRelationshipsAdded(const Sg::EventData<Sg::NodePtr, std::string, Sg::NodePtr> &relationships)
{
    relationaddedsubject = std::get<0>(relationships[0])->name();
    relationaddedpredicate = std::get<1>(relationships[0]);
    relationaddedobject = std::get<2>(relationships[0])->name();
}

void
onRelationshipsRemoved(const Sg::EventData<Sg::NodePtr, std::string, Sg::NodePtr> &relationships)
{
    relationremovedsubject = std::get<0>(relationships[0])->name();
    relationremovedpredicate = std::get<1>(relationships[0]);
    relationremovedobject = std::get<2>(relationships[0])->name();
}

struct GlobalInit
{
    GlobalInit()
    {
        Sg::initialize();
        Sg::registerCreator("camera", createCamera);
        Sg::registerCreator("group", createGroup);
        Sg::registerRelationshipType(Sg::TypedRelationshipType<Group, Sg::Node>("contains"));
    }
};

BOOST_GLOBAL_FIXTURE( GlobalInit );

BOOST_AUTO_TEST_CASE ( NodeCreatedTest )
{
    Sg::registerCallback<Sg::NodePtr>("nodesCreated", &onNodesCreated);

    BOOST_CHECK_EQUAL (createdoidname, "");
    const auto name = Sg::createNode("/interactive0", "camera")->name();
    BOOST_CHECK_EQUAL (createdoidname, name);
}

BOOST_AUTO_TEST_CASE ( NodeDeletedTest )
{
    Sg::registerCallback<std::string>("nodesDeleted", &onNodesDeleted);

    const auto name = Sg::createNode("/interactive0", "camera")->fullName();
    BOOST_CHECK_EQUAL (deletedoidname, "");
    Sg::deleteNode(name);
    BOOST_CHECK_EQUAL (deletedoidname, name);
}

BOOST_AUTO_TEST_CASE ( AttrCreatedTest )
{
    Sg::registerCallback<Sg::NodePtr, Sg::AttributePtr>("attrsCreated", &onAttrsCreated);

    auto node = Sg::createNode("/interactive1", "camera");

    BOOST_CHECK_EQUAL (createdattrname, "");

    node->addAttribute("fov", Sg::createAttribute("float"));
    BOOST_CHECK_EQUAL (createdattrname, "/" + node->name() + ":fov");
}

BOOST_AUTO_TEST_CASE ( AttrChangedTest )
{
    Sg::registerCallback<Sg::NodePtr, Sg::AttributePtr>("attrsChanged", &onAttrsChanged);

    auto node = Sg::createNode("/interactive2", "camera");

    node->addAttribute("fov", Sg::createAttribute("float"));

    BOOST_CHECK_EQUAL (changedattrname, "");
    node->setAttribute("fov", 15.0f);
    BOOST_CHECK_EQUAL (changedattrname, "/" + node->name() + ":fov");
}

BOOST_AUTO_TEST_CASE ( AttrDeletedTest )
{
    Sg::registerCallback<Sg::NodePtr, std::string>("attrsDeleted", &onAttrsDeleted);

    auto node = Sg::createNode("/interactive3", "camera");

    node->addAttribute("fov", Sg::createAttribute("float"));

    BOOST_CHECK_EQUAL (deletedattrname, "");
    node->delAttribute("fov");
    BOOST_CHECK_EQUAL (deletedattrname, "/" + node->name() + ":fov");
}

BOOST_AUTO_TEST_CASE ( RelationAddedTest )
{
    Sg::registerCallback<Sg::NodePtr, std::string, Sg::NodePtr>("relationshipsAdded", &onRelationshipsAdded);

    const auto grpa = Sg::createNode("/grpa1", "group");
    const auto grpb = Sg::createNode("/grpb1", "group");

    BOOST_CHECK_EQUAL (relationaddedsubject, "");
    BOOST_CHECK_EQUAL (relationaddedpredicate, "");
    BOOST_CHECK_EQUAL (relationaddedobject, "");
    Sg::makeTrue(grpa, "contains", grpb);
    BOOST_CHECK_EQUAL (relationaddedsubject, "grpa1");
    BOOST_CHECK_EQUAL (relationaddedpredicate, "contains");
    BOOST_CHECK_EQUAL (relationaddedobject, "grpb1");
}

BOOST_AUTO_TEST_CASE ( RelationRemovedTest )
{
    Sg::registerCallback<Sg::NodePtr, std::string, Sg::NodePtr>("relationshipsRemoved", &onRelationshipsRemoved);

    const auto grpa = Sg::createNode("/grpa2", "group");
    const auto grpb = Sg::createNode("/grpb2", "group");

    Sg::makeTrue(grpa, "contains", grpb);

    BOOST_CHECK_EQUAL (relationremovedsubject, "");
    BOOST_CHECK_EQUAL (relationremovedpredicate, "");
    BOOST_CHECK_EQUAL (relationremovedobject, "");
    Sg::makeFalse(grpa, "contains", grpb);
    BOOST_CHECK_EQUAL (relationremovedsubject, "grpa2");
    BOOST_CHECK_EQUAL (relationremovedpredicate, "contains");
    BOOST_CHECK_EQUAL (relationremovedobject, "grpb2");
}
