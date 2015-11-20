/*
 * typedAttribute.h
 *
 *  Created on: Aug 2, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGTYPEDATTRIBUTE_H
#define SGTYPEDATTRIBUTE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>
#include <sstream>
#include <string>
#include <typeinfo>
#include <memory>

#include <ut/utils.h>

#include "classPointers.h"
#include "timestamp.h"
#include "attribute.h"
#include "driver.h"
#include "constants.h"
#include "exception.h"
#include "functor.h"
#include "lambdaFunctor.h"
#include "ostreamhelpers.h"

namespace Sg
{

template <class T, typename... Args>
class TypedAttribute;

}

namespace Core
{

class Archive;

template<class T, typename... Args>
void serialize(Archive &ar, Sg::TypedAttribute<T, Args...> &t);

}

namespace Sg
{

/** Template base class for typed attributes.
  */

template <class T, class Derived, typename... Args>
class TypedAttributeBase : public Attribute,
                           public Driver
{
  public:
    /** Default Constructor. */
    TypedAttributeBase(const std::string &typeName, Serializable serializable=Serializable::TRUE);

    /** Constructor.
      * @param value initial value.
      */
    TypedAttributeBase(const std::string &typeName, const T &value, Serializable serializable=Serializable::TRUE);

    /** Constructor.
      * @param value initial value.
      */
    TypedAttributeBase(const std::string &typeName, T &&value, Serializable serializable=Serializable::TRUE);

    /** Copy Constructor.
      * @param other value to copy.
      */
    TypedAttributeBase(Derived &other);

    /** Copy Constructor.
      * @param other value to copy.
      */
    TypedAttributeBase(const Derived &other);

    /** Destructor. */
    virtual ~TypedAttributeBase();

    /** Assignment Operator.
      * @param other value to assign from.
      */
    virtual Attribute &                 operator =(Attribute &other);

    /** Assignment Operator.
      * @param other value to assign from.
      */
    Attribute &                         operator =(Derived &other);

    /** Get the internal timestamp of this attribute */
    virtual const Timestamp &           timestamp() const;

    /** Return const reference to value of attribute at time.
      * @param time float time at which to return value.
      */
    const T &                           value(const Args... args) const;

    /** Set value of attribute at time.
      * @param value const reference to type T to set attribute to.
      * @param time float time at which to set value.
      */
    void                                set(const T &value, const Args... args);
    void                                set(T &&value, const Args... args);
    void                                setKey(const T &value, const Args... args);
    void                                unsetKey(const Args... args);

    virtual bool                        hasKeys() const;
    virtual bool                        hasDriver() const;
    virtual DriverType                  driverType() const;
    virtual void                        setDriver(const DriverPtr &driver);

    template <typename D>
    void                                setDriver(const std::unique_ptr<D> &driver);

    virtual void                        clearDriver(DriverType type);

    virtual std::string                 toString() const = 0;

  protected:
    // Attribute value
    T                                   m_value;

  private:
    // Driver pointers
    std::exempt_ptr<Derived>                    m_attrDriver;
    std::exempt_ptr<Functor<T, Args...>>        m_funcDriver;
    std::exempt_ptr<LambdaFunctor<T, Args...>>  m_lambdaFuncDriver;

    // give ostream <<() access
    template<typename U, typename... UArgs>
    friend std::ostream&                operator <<(std::ostream &out, const Derived &attr);

    // give serialize() access
    template<class U, typename... UArgs>
    friend void Core::serialize(Core::Archive &ar, Sg::TypedAttribute<U, UArgs...> &t);
};

template <class T, class Derived, typename... Args>
TypedAttributeBase<T, Derived, Args...>::TypedAttributeBase(const std::string &typeName, Serializable serializable) :
    Attribute(typeid(T), typeName, serializable),
    m_value(T())
{
    m_timestamp.set();
}

template <class T, class Derived, typename... Args>
TypedAttributeBase<T, Derived, Args...>::TypedAttributeBase(const std::string &typeName, const T &value, Serializable serializable) :
    Attribute(typeid(T), typeName, serializable),
    m_value(value)
{
    m_timestamp.set();
}

template <class T, class Derived, typename... Args>
TypedAttributeBase<T, Derived, Args...>::TypedAttributeBase(const std::string &typeName, T &&value , Serializable serializable) :
    Attribute(typeid(T), typeName, serializable),
    m_value(std::move(value))
{
    m_timestamp.set();
}

template <class T, class Derived, typename... Args>
TypedAttributeBase<T, Derived, Args...>::TypedAttributeBase(Derived &other) :
    Attribute(typeid(T), other.m_typeName),
    m_value(std::move(other.m_value)),
    m_attrDriver(other.m_attrDriver),
    m_funcDriver(other.m_funcDriver),
    m_lambdaFuncDriver(other.m_lambdaFuncDriver)
{
    m_timestamp.set();
}

template <class T, class Derived, typename... Args>
TypedAttributeBase<T, Derived, Args...>::TypedAttributeBase(const Derived &other) :
    Attribute(typeid(T), other.m_typeName),
    m_value(std::move(other.m_value)),
    m_attrDriver(other.m_attrDriver),
    m_funcDriver(other.m_funcDriver),
    m_lambdaFuncDriver(other.m_lambdaFuncDriver)
{
    m_timestamp.set();
}

template <class T, class Derived, typename... Args>
TypedAttributeBase<T, Derived, Args...>::~TypedAttributeBase()
{
}

template <class T, class Derived, typename... Args>
Attribute &
TypedAttributeBase<T, Derived, Args...>::operator =(Attribute &other)
{
    if (this != &other)
    {
        try
        {
            auto &t = dynamic_cast<Derived &> (other);
            *this = t;
        }
        catch (const std::bad_cast &e)
        {
            const auto &othertype = other.type();
            const auto &thistype = type();
            THROW (TypeExc, "TypedAttribute<T>::cast(): Cannot cast attribute of type \"" << UNMANGLE(othertype) << "\" to type \"" << UNMANGLE(thistype) << "\".");
        }
    }

    return *(static_cast<Attribute*>(this));
}

template <class T, class Derived, typename... Args>
Attribute &
TypedAttributeBase<T, Derived, Args...>::operator =(Derived &other)
{
    if (this != &other)
    {
        *this = Attribute::operator =(other);
        m_value = std::move(other.m_value);
        m_attrDriver = other.m_attrDriver;
        m_funcDriver = other.m_funcDriver;
        m_lambdaFuncDriver = other.m_lambdaFuncDriver;
    }
    return *this;
}

template <class T, class Derived, typename... Args>
const Timestamp &
TypedAttributeBase<T, Derived, Args...>::timestamp() const
{
    if (m_funcDriver != nullptr)
        return m_funcDriver->timestamp();
    else if (m_lambdaFuncDriver != nullptr)
        return m_lambdaFuncDriver->timestamp();
    return m_timestamp;
}


template <class T, class Derived, typename... Args>
const T &
TypedAttributeBase<T, Derived, Args...>::value(const Args... args) const
{
    if (m_funcDriver != nullptr)
    {
        setTimestamp();
        return m_funcDriver->value(args...);
    }
    else if (m_lambdaFuncDriver != nullptr)
    {
        setTimestamp();
        return m_lambdaFuncDriver->value(args...);
    }
    else if (m_attrDriver != nullptr)
        return m_attrDriver->value(args...);

    return m_value;
}

template <class T, class Derived, typename... Args>
void
TypedAttributeBase<T, Derived, Args...>::set(const T &value, const Args... args)
{
    if (hasDriver())
        THROW (SyntaxExc, "TypedAttribute<T>::set(): Cannot set the value of a driven attribute.");
    else
        m_value = value;

    setTimestamp();
}

template <class T, class Derived, typename... Args>
void
TypedAttributeBase<T, Derived, Args...>::set(T &&value, const Args... args)
{
    if (hasDriver())
        THROW (SyntaxExc, "TypedAttribute<T>::set(): Cannot set the value of a driven attribute.");
    else
        m_value = std::move(value);

    setTimestamp();
}


template <class T, class Derived, typename... Args>
void
TypedAttributeBase<T, Derived, Args...>::setKey(const T &value, const Args... args)
{
}

template <class T, class Derived, typename... Args>
void
TypedAttributeBase<T, Derived, Args...>::unsetKey(const Args... args)
{
}

template <class T, class Derived, typename... Args>
bool
TypedAttributeBase<T, Derived, Args...>::hasKeys() const
{
    return false;
}

template <class T, class Derived, typename... Args>
bool
TypedAttributeBase<T, Derived, Args...>::hasDriver() const
{
    return (m_funcDriver != nullptr || m_lambdaFuncDriver != nullptr || m_attrDriver != nullptr);
}

template <class T, class Derived, typename... Args>
DriverType
TypedAttributeBase<T, Derived, Args...>::driverType() const
{
    if (m_funcDriver != nullptr || m_lambdaFuncDriver != nullptr)
        return (DriverType::FUNCTOR);
    else if (m_attrDriver != nullptr)
        return (DriverType::ATTRIBUTE);
    else
        return (DriverType::NONE);
}

template <class T, class Derived, typename... Args>
void
TypedAttributeBase<T, Derived, Args...>::setDriver(const DriverPtr &driver)
{
    if (auto p = std::exempt_pointer_cast<Functor<T, Args...>> (driver))
    {
        m_funcDriver = p;
        m_lambdaFuncDriver.reset();
        m_attrDriver.reset();
    }

    else if (auto p = std::exempt_pointer_cast<LambdaFunctor<T, Args...>> (driver))
    {
        m_lambdaFuncDriver = p;
        m_funcDriver.reset();
        m_attrDriver.reset();
    }

    else if (auto p = std::exempt_pointer_cast<Derived> (driver))
    {
        m_attrDriver = p;
        m_funcDriver.reset();
        m_lambdaFuncDriver.reset();
    }
}

template <class T, class Derived, typename... Args>
template <typename D>
void
TypedAttributeBase<T, Derived, Args...>::setDriver(const std::unique_ptr<D> &driver)
{
    setDriver(DriverPtr(driver.get()));
}

template <class T, class Derived, typename... Args>
void
TypedAttributeBase<T, Derived, Args...>::clearDriver(DriverType type)
{
    m_funcDriver.reset();
    m_lambdaFuncDriver.reset();
    m_attrDriver.reset();
}

// general case
template <class T, typename... Args>
class TypedAttribute : public TypedAttributeBase<T, TypedAttribute<T, Args...>, Args...>
{
  public:
    virtual ~TypedAttribute() {};
};

}

#endif
