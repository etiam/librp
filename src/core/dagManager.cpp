
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <boost/uuid/uuid_io.hpp>

#include "exception.h"
#include "relation.h"
#include "dagManager.h"

namespace Sg
{

void
DagManager::addNode(const RelationTypePtr &node)
{
    auto ret = m_vertices.insert(std::make_pair(node->uuid(), Vertex()));
    if (ret.second)
        ret.first->second = add_vertex(m_graph);
}

void
DagManager::deleteNode(const RelationTypePtr &node)
{
    const auto i = m_vertices.find(node->uuid());
    if (i == m_vertices.end())
       THROW (ArgExc, "DagManager::deleteNode(): DAG has no node " << node->uuid() << ".");

    // remove all edges to/from this node
    clear_vertex(i->second, m_graph);

    // remove node
    remove_vertex(i->second, m_graph);
}

void
DagManager::addEdge(const RelationTypePtr &nodeA, const RelationTypePtr &nodeB)
{
    const auto a = m_vertices.find(nodeA->uuid());
    if (a == m_vertices.end())
       THROW (ArgExc, "DagManager::addEdge(): DAG has no node " << nodeA->uuid() << ".");

    const auto b = m_vertices.find(nodeB->uuid());
    if (b == m_vertices.end())
       THROW (ArgExc, "DagManager::addEdge(): DAG has no node " << nodeB->uuid() << ".");

    add_edge(a->second, b->second, m_graph);

    bool hasCycle = false;
    cycleDetector vis(hasCycle);
    depth_first_search(m_graph, visitor(vis));

    if (hasCycle)
       THROW (SyntaxExc, "DagManager::addEdge(): cycle detected.");
}

void
DagManager::deleteEdge(const RelationTypePtr &nodeA, const RelationTypePtr &nodeB)
{
    const auto a = m_vertices.find(nodeA->uuid());
    if (a == m_vertices.end())
       THROW (ArgExc, "DagManager::deleteEdge(): DAG has no node " << nodeA->uuid() << ".");

    const auto b = m_vertices.find(nodeB->uuid());
    if (b == m_vertices.end())
       THROW (ArgExc, "DagManager::deleteEdge(): DAG has no node " << nodeB->uuid() << ".");

    remove_edge(a->second, b->second, m_graph);
}

}
