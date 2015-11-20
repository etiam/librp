/*
 * object.cpp
 *
 *  Created on: Mar 27, 2012
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "exception.h"
#include "object.h"

namespace Sg
{

Object::~Object()
{
}

void
Object::addAttribute(const std::string &name, std::unique_ptr<Attribute> attribute)
{
    if (m_attrs.find(name) != std::end(m_attrs))
        THROW (KeyExc, "Sg::Object::addAttribute(): attribute named \"" << name << "\" already exists.");

    m_attrs[name] = std::move(attribute);
    m_attrs[name]->setTimestamp();
}

void
Object::delAttribute(const std::string &name)
{
    if (m_attrs.find(name) == std::end(m_attrs))
        THROW (KeyExc, "Sg::Object::delAttribute(): No attribute named \"" << name << "\" to delete.");
    m_attrs.erase(name);
}

AttributePtr
Object::attributeByName(const std::string &name) const
{
    auto i = m_attrs.find(name);
    if (i == std::end(m_attrs))
        THROW (KeyExc, "Sg::Object::attributeByName(): No attribute named \"" << name << "\" found.");

    return i->second.get();
}

std::unique_ptr<Attribute> &
Object::attributePtrByName(const std::string &name)
{
    auto i = m_attrs.find(name);
    if (i == std::end(m_attrs))
        THROW (KeyExc, "Sg::Object::attributePtrByName(): No attribute named \"" << name << "\" found.");

    return i->second;
}

bool
Object::hasAttribute(const std::string &name) const
{
    return m_attrs.find(name) != std::end(m_attrs);
}

std::map<std::string, AttributePtr>
Object::attributeMap() const
{
    std::map<std::string, AttributePtr> result;

    for (const auto &attr : m_attrs)
        result[attr.first] = attr.second.get();

    return result;
}

std::ostream &
operator <<(std::ostream &out, const AttributeMap &map)
{
    std::stringstream s;

    s << "Sg::AttributeMap:" << std::endl;
    for (const auto &it : map)
    {
        s << " attrname    = " << it.first << std::endl;
        s << " attrvalue   = |" << std::endl;
        s << *it.second << std::endl;
    }

    return out << s.str();
}

}
