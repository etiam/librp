#pragma once
#ifndef SGOBJECT_H
#define SGOBJECT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>
#include <map>
#include <typeinfo>
#include <ut/utils.h>

#include "classPointers.h"
#include "logging.h"
#include "typedAttribute.h"
#include "ostreamhelpers.h"

namespace Sg
{

using AttrList     = std::vector<AttributePtr>;
using AttributeMap = std::map<std::string, AttributePtr>;

/** Base class for objects which can contain attributes.
  * Any class derived from Object can have attributes.
  * All attribute handling code is handled by this class.
  */

class Object
{
  public:
    Object() = default;
    virtual ~Object();

    virtual void                    addAttribute(const std::string &name, std::unique_ptr<Attribute> attribute);

    virtual void                    delAttribute(const std::string &name);

    template <typename T>
    void                            setAttribute(const std::string &name, T &value);

    template <typename T>
    void                            setAttribute(const std::string &name, const T &value);

    template <typename T>
    void                            setAttribute(const std::string &name, const TypedAttribute<T> &attribute);

    AttributePtr                    attributeByName(const std::string &name) const;

    std::unique_ptr<Attribute> &    attributePtrByName(const std::string &name);

    bool                            hasAttribute(const std::string &name) const;

    template <typename T>
    std::vector<std::exempt_ptr<TypedAttribute<T>>> attributesByType() const;

    AttributeMap                    attributeMap() const;

  private:
    using AttributeMap = std::map<std::string, std::unique_ptr<Attribute>>;

    AttributeMap                    m_attrs;
};

template <typename T>
void
Object::setAttribute(const std::string &name, T &value)
{
    if (m_attrs.find(name) == m_attrs.end())
        THROW (KeyExc, "Sg::Object::setAttribute() : No attribute named \"" << name << "\" found.");

    auto typed = dynamic_cast<TypedAttribute<T> *>(m_attrs.at(name).get());
    if (typed == nullptr)
    {
        THROW (TypeExc, "Sg::Object::setAttribute() : Cannot set attribute named \"" << name << "\" of type \"" <<
                        m_attrs.at(name)->type() << "\" to type \"" << typeid(value) << "\".");
    }

    LSGC0 << "Sg::Object::setAttribute(\"" << name << "\", " << value << ")";
    typed->set(std::move(value));
    m_attrs.at(name)->setTimestamp();
}

template <typename T>
void
Object::setAttribute(const std::string &name, const T &value)
{
    if (m_attrs.find(name) == m_attrs.end())
        THROW (KeyExc, "Sg::Object::setAttribute() : No attribute named \"" << name << "\" found.");

    auto typed = dynamic_cast<TypedAttribute<T> *>(m_attrs.at(name).get());
    if (typed == nullptr)
    {
        THROW (TypeExc, "Sg::Object::setAttribute() : Cannot set attribute named \"" << name << "\" of type \"" <<
                        m_attrs.at(name)->type() << "\" to type \"" << typeid(value) << "\".");
    }

    LSGC0 << "Sg::Object::setAttribute(\"" << name << "\", " << value << ")";
    typed->set(value);
    m_attrs.at(name)->setTimestamp();
}

template <typename T>
void
Object::setAttribute(const std::string &name, const TypedAttribute<T> &attribute)
{
    setAttribute(name, attribute.value());
}

template <typename T>
std::vector<std::exempt_ptr<TypedAttribute<T>>>
Object::attributesByType(void) const
{
    std::vector<std::exempt_ptr<TypedAttribute<T>>> attrs;

    for (const auto &attr : m_attrs)
    {
        auto castattr = dynamic_cast<TypedAttribute<T> *>(attr.second.get());
        if (castattr)
            attrs.push_back(castattr);
    }

    return attrs;
}

std::ostream & operator <<(std::ostream &out, const AttributeMap &map);

}

#endif
