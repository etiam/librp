#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <glob.h>
#include <dlfcn.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include "global.h"
#include "exception.h"
#include "timer.h"
#include "dagManager.h"
#include "eventManager.h"
#include "relationshipManager.h"
#include "node.h"

namespace Sg
{

RelationshipPtr
RelationshipManager::makeTrue(const NodePtr &subject, const std::string &predicate, const NodePtr &object)
{
    assert(subject != nullptr);
    assert(object != nullptr);
    assert(predicate != "");

    bool valid = false;

    for(const auto &reltype : m_relationshipTypes)
    {
        if (reltype->predicate() == predicate && reltype->check(*subject, *object))
        {
            valid = true;
            auto cmp = [&](const std::unique_ptr<Relationship> &rel)
                       {
                            return rel->subject() == subject &&
                                   rel->object() == object &&
                                   rel->type()->predicate() == predicate;
                       };
            const auto &it = std::find_if(std::begin(m_relationships), std::end(m_relationships), cmp);

            // don't allow same relationship to be added twice
            if (it != std::end(m_relationships))
                return it->get();

            m_relationships.push_back(std::make_unique<Relationship>(subject, reltype.get(), object));
//            dagManager().addEdge(subject, object);
            break;
        }
    }
    if (!valid)
        THROW (TypeExc, "RelationshipManager::makeTrue(): No relationtype \"" << predicate <<
                        "\" exists between type \"" << subject->type() << "\" and type \"" <<
                        object->type() << "\".");

    eventManager().emit<NodePtr, std::string, NodePtr>("relationshipsAdded", subject, predicate, object);

    return m_relationships.back().get();
}

void
RelationshipManager::makeFalse(const NodePtr &subject, const std::string &predicate, const NodePtr &object)
{
    assert(subject != nullptr);
    assert(object != nullptr);
    assert(predicate != "");

    bool valid = false;

    for(const auto &reltype : m_relationshipTypes)
    {
        if (reltype->predicate() == predicate && reltype->check(*subject, *object))
        {
            valid = true;
            auto cmp = [&](const std::unique_ptr<Relationship> &rel)
                       {
                            return rel->subject() == subject &&
                                   rel->object() == object &&
                                   rel->type()->predicate() == predicate;
                       };
            const auto &it = std::find_if(std::begin(m_relationships), std::end(m_relationships), cmp);
            if (it != std::end(m_relationships))
            {
                m_relationships.erase(it);
//                dagManager().deleteEdge(subject, object);
                break;
            }
        }
    }

    eventManager().emit<NodePtr, std::string, NodePtr>("relationshipsRemoved", subject, predicate, object);

    if (!valid)
        THROW (TypeExc, "RelationshipManager::makeFalse(): No relationtype \"" << predicate <<
                        "\" exists between type \"" << subject->type() << "\" and type \"" <<
                        object->type() << "\".");
}

const RelationshipPtr
RelationshipManager::getRelationship(const NodePtr &subject, const std::string &predicate, const NodePtr &object) const
{
    LSGC1 << "Sg::RelationshipManager::getRelationship()";
    Log::incIndent();
    LSGC2 << "m_relationships.size() = " << m_relationships.size();

    auto cmp = [&](const std::unique_ptr<Relationship> &rel)
               {
                    return rel->subject() == subject &&
                           rel->object() == object &&
                           rel->type()->predicate() == predicate;
               };
    Timer t;
    t.start();
    const auto &it = std::find_if(std::begin(m_relationships), std::end(m_relationships), cmp);
    t.stop();
    LSGC3 << "std::find_if() time = " << t.elapsedTime() << "ms";

    if (it == std::end(m_relationships))
        THROW (KeyExc, "RelationshipManager::getRelationship(): No relationship\"" << subject->type() << "\"" <<
                       "\"" << predicate << "\" \"" << object->type() << "\" found.");

    Log::decIndent();
    return it->get();
}

NodeList
RelationshipManager::listSubjects(const NodePtr &object, const std::string &predicate) const
{
    NodeList subjects;

    for(const auto &rel : m_relationships)
        if (rel->type()->predicate() == predicate && rel->object() == object)
            if (auto node = std::exempt_pointer_cast<Node>(rel->subject()))
                subjects.push_back(node);

    return subjects;
}

NodeList
RelationshipManager::listObjects(const NodePtr &subject, const std::string &predicate) const
{
    NodeList objects;

    for(const auto &rel : m_relationships)
        if (rel->type()->predicate() == predicate && rel->subject() == subject)
            if (auto node = std::exempt_pointer_cast<Node>(rel->object()))
                objects.push_back(node);

    return objects;
}

Relationships &
RelationshipManager::relationships()
{
    return m_relationships;
}

std::vector<RelationshipPtr>
RelationshipManager::relationships(const std::string &path) const
{
    std::vector<RelationshipPtr> results;
    auto cpath = path.c_str();

    for(const auto &rel : m_relationships)
    {
        if (Ut::wildCompare(cpath, std::exempt_pointer_cast<Node>(rel->subject())->fullName().c_str()) ||
            Ut::wildCompare(cpath, std::exempt_pointer_cast<Node>(rel->object())->fullName().c_str()))
            results.push_back(rel.get());
    }
    return results;
}


} // namespace Sg
