#pragma once
#ifndef SGRELATIONSHIP_H
#define SGRELATIONSHIP_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>

#include "classPointers.h"
#include "object.h"

namespace Sg
{

class Relationship : public Object
{
  public:
    Relationship(const RelationTypePtr &subject, const RelationshipTypePtr &type, const RelationTypePtr &object);
    virtual ~Relationship();

    const RelationTypePtr &         subject(void) const;
    const RelationTypePtr &         object(void) const;
    const RelationshipTypePtr &     type(void) const;

  private:
    RelationTypePtr                 m_subject;
    RelationTypePtr                 m_object;
    RelationshipTypePtr             m_type;
};

}

#endif
