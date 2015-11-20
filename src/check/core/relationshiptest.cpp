/*
 * relationtest.cpp
 *
 *  Created on: Jan 27, 2011
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RelationshipTest
#include <boost/test/unit_test.hpp>
#include <ut/make_unique.h>

#include "core/global.h"
#include "core/relationship.h"
#include "core/relationshipManager.h"
#include "core/eventManager.h"
#include "core/node.h"
#include "core/standardAttributes.h"
#include "core/ostreamhelpers.h"

class Group : public Sg::Node
{
  public:
    Group() = default;
    ~Group() = default;

    virtual std::string     nodeType() const { return "group"; }
    virtual void            initializeNode() {}
};

std::unique_ptr<Sg::Node>
createGroup()
{
    return std::make_unique<Group>();
}

struct GlobalInit
{
    GlobalInit()
    {
        Sg::initialize();
        Sg::registerCreator("group", createGroup);
        Sg::registerRelationshipType(Sg::TypedRelationshipType<Group, Sg::Node>("contains"));
    }
};

BOOST_GLOBAL_FIXTURE( GlobalInit );

BOOST_AUTO_TEST_CASE ( MakeTrueTest )
{
    const auto grpa = Sg::createNode("/grpa2", "group");
    const auto grpb = Sg::createNode("/grpb2", "group");

    auto &relMgr = Sg::relationshipManager();

    auto &&rel = relMgr.makeTrue(grpa, "contains", grpb);
    auto stringattr = Sg::asStringAttribute(Sg::createAttribute("string"));
    stringattr->set("A");
    rel->addAttribute("inputname", std::move(stringattr));

    BOOST_CHECK_EQUAL (relMgr.listObjects(grpa, "contains")[0], grpb);
    BOOST_CHECK_EQUAL (relMgr.listSubjects(grpb, "contains")[0], grpa);

    BOOST_CHECK_EQUAL (relMgr.listObjects<Group>(grpa, "contains")[0], grpb);
    BOOST_CHECK_EQUAL (relMgr.listSubjects<Group>(grpb, "contains")[0], grpa);

    auto &rel2 = relMgr.getRelationship(grpa, "contains", grpb);
    BOOST_CHECK_EQUAL (rel, rel2);

    BOOST_CHECK_EQUAL (asStringAttribute(rel2->attributeByName("inputname"))->value(), "A");
}

BOOST_AUTO_TEST_CASE ( MakeFalseTest )
{
    const auto grpa = Sg::createNode("/grpa2", "group");
    const auto grpb = Sg::createNode("/grpb2", "group");

    auto &relMgr = Sg::relationshipManager();

    relMgr.makeFalse(grpa, "contains", grpb);

    BOOST_CHECK_EQUAL (relMgr.listObjects(grpa, "contains").empty(), true);
    BOOST_CHECK_EQUAL (relMgr.listSubjects(grpb, "contains").empty(), true);
    BOOST_CHECK_THROW (relMgr.getRelationship(grpa, "contains", grpb), Sg::KeyExc);
}

//BOOST_AUTO_TEST_CASE ( CycleTest )
//{
//    const auto grpa = Sg::createNode("/grpa2", "group");
//    const auto grpb = Sg::createNode("/grpb2", "group");
//
//    auto &relMgr = Sg::relationshipManager();
//
//    relMgr.makeTrue(grpa, "contains", grpb);
//    BOOST_CHECK_THROW (relMgr.makeTrue(grpb, "contains", grpa), Sg::SyntaxExc);
//}
