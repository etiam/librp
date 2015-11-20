#pragma once
#ifndef SGRELATIONSHIPMGR_H
#define SGRELATIONSHIPMGR_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>
#include <vector>
#include <boost/utility.hpp>
#include <ut/make_unique.h>

#include "classPointers.h"
#include "master.h"
#include "nodeManager.h"
#include "relationship.h"
#include "relationshipType.h"

namespace Sg
{

using Relationships = std::vector<std::unique_ptr<Relationship>>;
using RelationshipTypes = std::vector<std::unique_ptr<RelationshipType>>;

class RelationshipManager : boost::noncopyable
{
  public:
    RelationshipManager() = default;
    ~RelationshipManager() = default;

    template <class S, class O>
    void                                    registerType(const TypedRelationshipType<S, O> &rel);

    RelationshipPtr                         makeTrue(const NodePtr &subject, const std::string &predicate, const NodePtr &object);
    void                                    makeFalse(const NodePtr &subject, const std::string &predicate, const NodePtr &object);

    const RelationshipPtr                   getRelationship(const NodePtr &subject, const std::string &predicate, const NodePtr &object) const;

    NodeList                                listSubjects(const NodePtr &object, const std::string &predicate) const;
    NodeList                                listObjects(const NodePtr &subject, const std::string &predicate) const;

    template <class T>
    std::vector<std::exempt_ptr<T>>         listSubjects(const NodePtr &object, const std::string &predicate) const;
    template <class T>
    std::vector<std::exempt_ptr<T>>         listObjects(const NodePtr &subject, const std::string &predicate) const;

    Relationships &                         relationships();
    std::vector<RelationshipPtr>            relationships(const std::string &path) const;

  private:
    friend std::ostream &                   operator <<(std::ostream &out, const RelationshipManager &mgr);

    Relationships                           m_relationships;
    RelationshipTypes                       m_relationshipTypes;
};

template <class S, class O>
void
RelationshipManager::registerType(const TypedRelationshipType<S, O> &reltype)
{
    auto &types = Master::relationshipManager().m_relationshipTypes;

    // don't add the same relationship type twice
    for (const auto &it : types)
        if (reltype == *it)
            return;

    types.push_back(std::make_unique<TypedRelationshipType<S, O>>(reltype.predicate()));
}

template <class T>
std::vector<std::exempt_ptr<T>>
RelationshipManager::listSubjects(const NodePtr &object, const std::string &predicate) const
{
    std::vector<std::exempt_ptr<T>> results;

    for (const auto &rel : Master::relationshipManager().listSubjects(object, predicate))
    {
        auto node = (std::exempt_pointer_cast<T>(rel));
        if (node)
            results.push_back(node);
    }

    return results;
}

template <class T>
std::vector<std::exempt_ptr<T>>
RelationshipManager::listObjects(const NodePtr &subject, const std::string &predicate) const
{
    std::vector<std::exempt_ptr<T>> results;

    for (const auto &rel : Master::relationshipManager().listObjects(subject, predicate))
    {
        auto node = std::exempt_pointer_cast<T>(rel);
        if (node)
            results.push_back(node);
    }

    return results;
}

}

#endif
