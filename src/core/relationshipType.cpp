/*
 * relationshipType.cpp
 *
 *  Created on: Mar 30, 2012
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "relationshipType.h"

namespace Sg {


RelationshipType::RelationshipType(const std::string &predicate,
                                   const std::type_info &subjectType,
                                   const std::type_info &objectType) :
	m_predicate(predicate),
	m_subjectType(subjectType),
	m_objectType(objectType)
{
}

RelationshipType::~RelationshipType()
{
}

const std::string &
RelationshipType::predicate(void) const
{
    return m_predicate;
}

const std::type_info &
RelationshipType::subjectType(void) const
{
    return m_subjectType;
}

const std::type_info &
RelationshipType::objectType(void) const
{
    return m_objectType;
}


}  // namespace Sg


