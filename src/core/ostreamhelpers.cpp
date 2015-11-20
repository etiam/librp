/*
 * ostreamhelpers.cpp
 *
 *  Created on: Mar 25, 2012
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cxxabi.h>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <boost/uuid/uuid_io.hpp>
#include <ut/utils.h>
#include <ut/exempt_ptr.h>

#include "node.h"
#include "object.h"
#include "attribute.h"
#include "relationship.h"
#include "relationshipType.h"
#include "relationshipManager.h"
#include "timestamp.h"
#include "ostreamhelpers.h"

namespace Sg
{

std::ostream &
operator <<(std::ostream &out, const Node &node)
{
    return out << node.fullName();
}

std::ostream &
operator <<(std::ostream &out, const Attribute &attr)
{
    return out << attr.name();
}

std::ostream &
operator <<(std::ostream &out, const Relationship &rel)
{
    return out << std::exempt_pointer_cast<const Node>(rel.subject()) << " " <<
                  rel.type()->predicate() << " " <<
                  std::exempt_pointer_cast<const Node>(rel.object()) ;
}

std::ostream &
operator <<(std::ostream &out, const Timestamp &ts)
{
    using namespace std::chrono;

    auto tp = ts.get();
    auto tt = high_resolution_clock::to_time_t(tp);
    auto tm = localtime(&tt);
    auto sec = tp - high_resolution_clock::from_time_t(tt) + seconds(tm->tm_sec);

    char mbstr[100];
    std::strftime(mbstr, 100, "%m-%d-%y %H:%M", std::localtime(&tt));
    std::stringstream str;
    str << mbstr << ":" << std::fixed << sec.count();

    return out << str.str();
}

//std::ostream &
//operator <<(std::ostream &out, const RelationType &rel)
//{
//    std::stringstream s;
//
//    s << "Sg::RelType:" << std::endl;
//    s << " type        = " << UNMANGLE(rel) << std::endl;
//    s << " uuid        = " << rel.m_uuid << std::endl;
//
//    return (out << s.str());
//}
//std::ostream &
//operator <<(std::ostream &out, const RelationshipType &rel)
//{
//    std::stringstream s;
//
//    s << UNMANGLE(rel.subjectType().name()) << " ";
//    s << rel.predicate() << " ";
//    s << UNMANGLE(rel.objectType().name());
//
//    return out << s.str();
//}
//
//std::ostream &
//operator <<(std::ostream &out, const RelationshipManager &mgr)
//{
//    std::stringstream s;
//
//    s << "Sg::RelationManager:" << std::endl;
//    s << " relations  = |" << std::endl;
//    for (const auto &i : mgr.m_relationships)
//        s << *i << std::endl;
//
//    s << " types      = |" << std::endl;
//    for (const auto &i : mgr.m_relationshipTypes)
//        s << *i << std::endl;
//
//    return out << s.str();
//}

}
