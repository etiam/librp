
/*
 * sgRelType.cpp
 *
 *  Created on: Jan 30, 2011
 *      Author: jasonr
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sstream>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <ut/utils.h>

#include "relation.h"

namespace Sg
{

Relation::Relation()
{
    m_type = UNMANGLE(*this);
    static boost::uuids::random_generator gen;
    m_uuid = gen();
}

Relation::~Relation ()
{
}

const std::string &
Relation::type(void) const
{
    return m_type;
}

const boost::uuids::uuid &
Relation::uuid() const
{
    return m_uuid;
}

bool
Relation::operator ==(const Relation &other) const
{
    return m_uuid == other.uuid();
}

}
