/*
 * attribute.cpp
 *
 *  Created on: Mar 29, 2012
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sys/timeb.h>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <string>
#include <typeinfo>
#include <memory>

#include "attribute.h"

namespace Sg
{

Attribute::Attribute(const std::type_info &type, const std::string &typeName, Serializable serializable) :
    m_serializable(serializable),
    m_type(type),
    m_typeName(typeName),
    m_uiOrder(0)
{
}

Attribute::~Attribute()
{
}

Attribute &
Attribute::operator =(Attribute &other)
{
    if (this != &other)
    {
        m_timestamp = other.m_timestamp;
        m_name = other.m_name;
        m_uiName = other.m_uiName;
        m_uiOrder = other.m_uiOrder;
    }

    return *this;
}

const std::type_info &
Attribute::type(void) const
{
    return m_type;
}

const std::string &
Attribute::typeName(void) const
{
    return m_typeName;
}

bool
Attribute::isDirty(const AttributePtr &attr) const
{
    return (!timestamp().valid() || attr->timestamp() >= timestamp());
}

void
Attribute::setTimestamp() const
{
    m_timestamp.set();
}

const std::string &
Attribute::name() const
{
    return m_name;
}

void
Attribute::setName(const std::string &name)
{
    m_name = name;
}

const std::string &
Attribute::uiName() const
{
    return m_uiName;
}

void
Attribute::setUiName(const std::string &uiName)
{
    m_uiName = uiName;
}

uint
Attribute::uiOrder() const
{
    return m_uiOrder;
}

void
Attribute::setUiOrder(uint uiOrder)
{
    m_uiOrder = uiOrder;
}

bool
Sg::Attribute::isSerializable() const
{
    return m_serializable == Serializable::TRUE;
}

bool
operator ==(const Attribute &lhs, const Attribute &rhs)
{
    return lhs.m_name == rhs.m_name;
}


} // namespace Sg
