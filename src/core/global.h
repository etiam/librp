/*
 *
 * global.h
 *
 *  Created on: Jul 1, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGGLOBAL_H_
#define SGGLOBAL_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cassert>
#include <set>
#include <ut/make_unique.h>

#include "classPointers.h"
#include "master.h"
#include "typedAttribute.h"
#include "nodeManager.h"
#include "relationshipManager.h"
#include "eventManager.h"
#include "nodeFactory.h"
#include "attributeFactory.h"

namespace Sg
{

class DagManager;
class SelectionManager;

// NodeManager convenience functions
bool                            exists(const std::string &path);
NodePtr                         addNode(const std::string &path, std::unique_ptr<Node> node);
void                            deleteNode(const std::string &path);
void                            deleteNode(const NodePtr &node);
NodePtr                         findNode(const std::string &path);
NodeList                        findNodes(const std::string& pattern);
void                            renameNode(const std::string &frompath, const std::string &topath);
const NodeList &                nodes();
size_t                          nodeCount();
void                            lockNodes();
void                            unlockNodes();

// SelectionManager convenience functions
void                            selectNode(const NodePtr &node);
const NodeList &                selectedNodes();
void                            clearSelection();

// RelationshipManager convenience functions
RelationshipPtr                 makeTrue(const NodePtr &subject, const std::string &predicate, const NodePtr &object);
void                            makeFalse(const NodePtr &subject, const std::string &predicate, const NodePtr &object);
NodeList                        listSubjects(const NodePtr &object, const std::string &predicate);
NodeList                        listObjects(const NodePtr &subject, const std::string &predicate);
Relationships &                 relationships();
std::vector<RelationshipPtr>    relationships(const std::string &path);

// EventManager convenience functions
void                            batchEvents(bool active);
void                            unregisterCallback(const Connection &connection);

// global init
void                            initialize();
void                            shutdown();

// Access to Managers
RelationshipManager &           relationshipManager();
NodeManager &                   nodeManager();
EventManager &                  eventManager();
DagManager &                    dagManager();
SelectionManager &              selectionManager();
NodeFactory &                   nodeFactory();
AttributeFactory &              attributeFactory();

// Create a Node of type at path and add it to NodeManager/DagManager.
template<typename ...Args>
NodePtr
createNode(const std::string &path, const std::string &type, Args&&... args)
{
    return addNode(path, Master::nodeFactory().create<Args...>(type, std::forward<Args>(args)...));
}

// Create Attribute of type.
template<typename ...Args>
std::unique_ptr<Attribute>
createAttribute(const std::string &type, Args&&... args)
{
    return std::move(Master::attributeFactory().create<Args...>(type, std::forward<Args>(args)...));
}

//
// NodeManager convenience functions
//

template <typename T>
std::exempt_ptr<T>
findNode(const std::string &path)
{
    return nodeManager().findNode<T>(path);
}

template <typename T>
std::vector<std::exempt_ptr<T>>
findNodes(const std::string &path)
{
    return nodeManager().findNodes<T>(path);
}

template <typename T>
std::vector<std::exempt_ptr<T>>
nodes()
{
    return nodeManager().nodes<T>();
}

//
// RelationshipManager convenience functions
//

template <class S, class O>
void
registerRelationshipType(const TypedRelationshipType<S, O> &reltype)
{
    relationshipManager().registerType<S, O>(reltype);
}

//
// EventManager convenience functions
//

template <typename... Args>
Connection
registerCallback(const std::string &name, const Callback<Args...> &callback)
{
    return eventManager().registerCallback<Args...>(name, callback);
}

template <typename... Args>
void
registerEventType(const std::string &name, bool canBatch=true)
{
    eventManager().registerEventType<Args...>(name, canBatch);
}

template <typename... Args>
void
emit(const std::string &name, const Args... args)
{
    eventManager().emit<Args...>(name, args...);
}

//
// NodeFactory convenience functions
//

template<typename ...Args>
void
registerCreator(const std::string &type, const std::function<std::unique_ptr<Node> (Args...)> &functor)
{
    nodeFactory().registerCreator<Args...>(type, functor);
}

template<typename ...Args>
void
registerCreator(const std::string &type, std::unique_ptr<Node> functor(Args...))
{
    nodeFactory().registerCreator(type, std::function<std::unique_ptr<Node> (Args...)>(functor));
}


template<typename Functor, typename ...Args>
void
registerCreator(const std::string &type, const Functor &functor)
{
    nodeFactory().registerCreator(type, std::function<std::unique_ptr<Node> (Args...)>(functor));
}

//
// TypedAttribute convenience functions
//

template <class T>
bool
isTypedAttribute(const AttributePtr &attr)
{
    return std::exempt_pointer_cast<const TypedAttribute<T>>(attr) != nullptr;
}

template <class T, typename... Args>
std::exempt_ptr<TypedAttribute<T, Args...>>
asTypedAttribute(const AttributePtr &attr)
{
    auto typedattr = std::exempt_pointer_cast<TypedAttribute<T, Args...>>(attr);
    assert(typedattr != nullptr);

    return typedattr;
}

template <class T, typename... Args>
std::unique_ptr<TypedAttribute<T, Args...>>
asTypedAttribute(std::unique_ptr<Attribute> attr)
{
    auto typedattr = std::unique_pointer_cast<TypedAttribute<T, Args...>, Attribute>(std::move(attr));
    assert(typedattr != nullptr);

    return typedattr;
}

template <class T, typename... Args>
std::unique_ptr<TypedAttribute<T, Args...>>
newAttribute()
{
    return std::make_unique<TypedAttribute<T, Args...>>();
}

template <class T, typename... Args>
std::unique_ptr<TypedAttribute<T, Args...>>
newAttribute(const T &value)
{
    return std::make_unique<TypedAttribute<T, Args...>>(value);
}

template <class T, typename... Args>
std::unique_ptr<TypedAttribute<T, Args...>>
newAttribute(T &&value)
{
    return std::make_unique<TypedAttribute<T, Args...>>(std::forward<T, Args...>(value));
}

template <class T, typename... Args>
std::unique_ptr<TypedAttribute<T, Args...>>
newAttribute(TypedAttribute<T, Args...> &attr)
{
    return std::make_unique<TypedAttribute<T, Args...>>(attr);
}

template <class T, typename... Args>
std::unique_ptr<TypedAttribute<T, Args...>>
newAttribute(const TypedAttribute<T, Args...> &attr)
{
    return std::make_unique<TypedAttribute<T, Args...>>(attr);
}

}

#endif // SGGLOBAL_H_
