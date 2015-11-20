/*
 * global.cpp
 *
 *  Created on: Aug 22, 2012
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cassert>
#include <utility>

#include "global.h"

#include "eventManager.h"
#include "dagManager.h"
#include "selectionManager.h"

namespace Sg
{

bool
exists(const std::string &path)
{
    return nodeManager().exists(path);
}

NodePtr
addNode(const std::string &path, std::unique_ptr<Node> node)
{
    assert(node != nullptr);

    auto ref = nodeManager().addNode(path, std::move(node));
//    dagManager().addNode(ref);

    // child classes of Node implement this to setup the node (addNode attrs, etc).
    ref->initializeNode();

    // emit "nodesCreated" signal
    eventManager().emit<NodePtr>("nodesCreated", ref);

    return ref;
}

void
deleteNode(const std::string &path)
{
    deleteNode(findNode(path));
}

void
deleteNode(const NodePtr &node)
{
    assert(node != nullptr);

    const auto fullname = node->fullName();

    eventManager().emit<NodePtr>("nodeDeletionRequested", node);

//    dagManager().deleteNode(node);
    nodeManager().deleteNode(node);

    eventManager().emit<std::string>("nodesDeleted", fullname);
}

NodePtr
findNode(const std::string &path)
{
    return nodeManager().findNode(path);
}

NodeList
findNodes(const std::string& pattern)
{
    return nodeManager().findNodes(pattern);
}

void
renameNode(const std::string &frompath, const std::string &topath)
{
    const auto tonode = nodeManager().renameNode(frompath, topath);

    // emit "nodeChanged" signal for changed node
    eventManager().emit<NodePtr, std::string, std::string>("nodesRenamed", std::get<1>(*tonode), "N/A", "N/A");

    // emit "nodeChanged" signal for any children nodes
    for (const auto &node : findNodes(topath + "/*"))
        eventManager().emit<NodePtr, std::string, std::string>("nodesRenamed", node, "N/A", "N/A");
}

const NodeList &
nodes()
{
    return nodeManager().nodes();
}

size_t
nodeCount()
{
    return nodeManager().nodeCount();
}

void
lockNodes()
{
    nodeManager().lock();
}

void
unlockNodes()
{
    nodeManager().unlock();
}

void
selectNode(const NodePtr &node)
{
    assert(node != nullptr);

    selectionManager().selectNode(node);
}

const NodeList &
selectedNodes()
{
    return selectionManager().selection();
}

void
clearSelection()
{
    selectionManager().clearSelection();
}

RelationshipPtr
makeTrue(const NodePtr& subject, const std::string& predicate, const NodePtr& object)
{
    return relationshipManager().makeTrue(subject, predicate, object);
}

void
makeFalse(const NodePtr& subject, const std::string& predicate, const NodePtr& object)
{
    return relationshipManager().makeFalse(subject, predicate, object);
}

NodeList
listSubjects(const NodePtr &object, const std::string &predicate)
{
    return relationshipManager().listSubjects(object, predicate);
}

NodeList
listObjects(const NodePtr &subject, const std::string &predicate)
{
    return relationshipManager().listObjects(subject, predicate);
}

Relationships &
relationships()
{
    return relationshipManager().relationships();
}

std::vector<RelationshipPtr>
relationships(const std::string &path)
{
    return relationshipManager().relationships(path);
}

void
batchEvents(bool active)
{
    eventManager().batchEvents(active);
}

void
unregisterCallback(const Connection &connection)
{
    eventManager().unregisterCallback(connection);
}

void
initialize()
{
    Master::initialize();
}

void
shutdown()
{
    Master::shutdown();
}


NodeManager &
nodeManager()
{
    return Master::nodeManager();
}

EventManager &
eventManager()
{
    return Master::eventManager();
}

RelationshipManager &
relationshipManager()
{
    return Master::relationshipManager();
}

DagManager &
dagManager()
{
    return Master::dagManager();
}

NodeFactory &
nodeFactory()
{
    return Master::nodeFactory();
}

AttributeFactory &
attributeFactory()
{
    return Master::attributeFactory();
}

SelectionManager &
selectionManager()
{
    return Master::selectionManager();
}

}
