#pragma once
#ifndef SGRELATIONTYPE_H
#define SGRELATIONTYPE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>
#include <boost/uuid/uuid.hpp>

#include "classPointers.h"
#include "constants.h"

namespace Sg
{

/** Abstract base class for objects which can be members of relations.
  * Any class derived from RelType can be either the subject or
  * object in a Relation.
  */

class Relation
{
  public:
    Relation();
    virtual ~Relation();

    /** Return type of object instance. */
    const std::string &                     type(void) const;

    /** Return uuid of object instance. */
    const boost::uuids::uuid &              uuid() const;

    bool                                    operator ==(const Relation &other) const;

  protected:
    std::string                             m_type;
    boost::uuids::uuid                      m_uuid;

    friend std::ostream&                    operator <<(std::ostream &out, const Relation &rel);
};

}

#endif
