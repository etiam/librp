#pragma once
#ifndef SGRELATIONSHIPTYPE_H
#define SGRELATIONSHIPTYPE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sstream>
#include <string>
#include <typeinfo>
#include <ut/utils.h>

#include "classPointers.h"
#include "relation.h"

namespace Sg
{

class RelationshipType
{
  public:
    RelationshipType(const std::string &predicate, const std::type_info &subjectType, const std::type_info &objectType);
    virtual ~RelationshipType();

    const std::string &             predicate() const;
    const std::type_info &          subjectType() const;
    const std::type_info &          objectType() const;

    /** Return true if the subject and object attr types match the instances types **/
    virtual bool                    check(const Relation &subject, const Relation &object) const = 0;

    virtual bool                    operator ==(const RelationshipType &other) const = 0;

  protected:
    std::string                     m_predicate;
    const std::type_info &          m_subjectType;
    const std::type_info &          m_objectType;
};

template <class S, class O>
class TypedRelationshipType : public RelationshipType
{
  public:
    TypedRelationshipType(const std::string &predicate);
    virtual ~TypedRelationshipType();


    virtual bool                    check(const Relation &subject, const Relation &object) const;

    virtual bool                    operator ==(const RelationshipType &other) const;
};

template <class S, class O>
TypedRelationshipType<S, O>::TypedRelationshipType(const std::string &predicate) :
    RelationshipType::RelationshipType(predicate, typeid(S), typeid(O))
{
}

template <class S, class O>
TypedRelationshipType<S, O>::~TypedRelationshipType()
{
}

template <class S, class O>
bool
TypedRelationshipType<S, O>::check(const Relation &subject, const Relation &object) const
{
    try
    {
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wunused-value"
        dynamic_cast <const S &>(subject);
        dynamic_cast <const O &>(object);
        #pragma clang diagnostic pop
    }
    catch (const std::bad_cast &e)
    {
        return false;
    }

    return true;
}


template <class S, class O>
bool
TypedRelationshipType<S, O>::operator ==(const RelationshipType &other) const
{
    return (m_predicate == other.predicate() &&
            m_subjectType == other.subjectType() &&
            m_objectType == other.objectType());
}

}

#endif
