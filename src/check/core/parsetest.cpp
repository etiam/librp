/*
 * parsetest.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EventTest
#include <boost/test/unit_test.hpp>

struct GlobalInit
{
    GlobalInit()
    {
    }
};

BOOST_GLOBAL_FIXTURE( GlobalInit );

BOOST_AUTO_TEST_CASE ( ParseTest )
{
    BOOST_CHECK_EQUAL (1, 1);
}
