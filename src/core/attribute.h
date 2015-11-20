#pragma once
#ifndef SGATTRIBUTE_H
#define SGATTRIBUTE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "classPointers.h"
#include "timestamp.h"
#include "constants.h"

namespace Sg
{

class Attribute;

}

namespace Core
{

class Archive;

void serialize(Archive &ar, Sg::Attribute &t);

}

namespace Sg
{

enum class DriverType : ushort
{
    NONE,
    FUNCTOR,
    ATTRIBUTE
};

enum class Serializable : bool
{
    TRUE,
    FALSE
};

/** Abstract base class for attribute objects. */

class Attribute
{
  protected:
    Attribute(const std::type_info &type, const std::string &typeName, Serializable serializable);

  public:
    virtual ~Attribute();

    virtual Attribute &         operator =(Attribute &other);

    const std::type_info &      type() const;

    const std::string &         typeName() const;

    /** Return whether attribute is keyed */
    virtual bool                hasKeys() const = 0;

    /** Return whether attribute is driven */
    virtual bool                hasDriver() const = 0;

    /** Return type of driver when attr is driven */
    virtual DriverType          driverType() const = 0;

    /** Make this attribute driven by a driver object.
      * @param driver: object which will drive this attribute.
      */
    virtual void                setDriver(const DriverPtr &driver) = 0;

    /** Clear drivers of the given type for this attr  */
    virtual void                clearDriver(DriverType type) = 0;

    /** Return whether attribute is dirty compared to attr */
    bool                        isDirty(const AttributePtr &attr) const;

    virtual const Timestamp &   timestamp() const = 0;

    /** Set the timestamp of this attribute to the current time */
    void                        setTimestamp() const;

    const std::string &         name() const;
    void                    	setName(const std::string &name);

    const std::string &         uiName() const;
    void                    	setUiName(const std::string &uiName);

    uint                    	uiOrder() const;
    void                    	setUiOrder(uint uiOrder);

    bool                        isSerializable() const;

    virtual std::string         toString() const = 0;

  protected:
    friend bool                 operator ==(const Attribute &lhs, const Attribute &rhs);

    Serializable                m_serializable;

    const std::type_info &      m_type;
    const std::string           m_typeName;

    mutable Timestamp           m_timestamp;

  private:
    std::string             	m_name;
    std::string             	m_uiName;
    uint                    	m_uiOrder;

    friend void Core::serialize(Core::Archive &ar, Attribute &t);
};

bool operator ==(const Attribute &lhs, const Attribute &rhs);
std::ostream & operator <<(std::ostream &out, const Attribute &attr);

}

#endif
