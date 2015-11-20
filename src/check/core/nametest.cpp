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
#define BOOST_TEST_MODULE NameTest
#include <boost/test/unit_test.hpp>
#include <ut/make_unique.h>

#include "core/global.h"
#include "core/relationship.h"
#include "core/relationshipManager.h"
#include "core/eventManager.h"
#include "core/node.h"
#include "core/standardAttributes.h"
#include "core/ostreamhelpers.h"

#include "core/master.h"

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

struct GlobalInit
{
    GlobalInit()
    {
        Sg::initialize();
        Sg::registerCreator("group", createGroup);
    }
};

BOOST_GLOBAL_FIXTURE( GlobalInit );

BOOST_AUTO_TEST_CASE ( PathTest )
{
    auto grpa1 = Sg::createNode("/etc/grpa/foo", "group");
    BOOST_CHECK_EQUAL (grpa1->name(), "foo");
    BOOST_CHECK_EQUAL (grpa1->fullName(), "/etc/grpa/foo");

    auto grpa2 = Sg::createNode("/etc/grpa/foo", "group");
    BOOST_CHECK_EQUAL (grpa2->name(), "foo1");
    BOOST_CHECK_EQUAL (grpa2->fullName(), "/etc/grpa/foo1");

    Sg::renameNode("/etc", "/etc1");
    BOOST_CHECK_EQUAL (grpa1->fullName(), "/etc1/grpa/foo");
    BOOST_CHECK_EQUAL (grpa2->fullName(), "/etc1/grpa/foo1");

    auto nodes = Sg::findNodes("/etc1/grp?/*");
    BOOST_CHECK_EQUAL (nodes.size(), 2);
    BOOST_CHECK_EQUAL (nodes[0]->name(), "foo");
    BOOST_CHECK_EQUAL (nodes[1]->name(), "foo1");
}

