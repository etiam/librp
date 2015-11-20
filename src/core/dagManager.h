#pragma once
#ifndef SGDAGMGR_H
#define SGDAGMGR_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/utility.hpp>

#include "classPointers.h"

namespace Sg
{

class DagManager : boost::noncopyable
{
  public:
    DagManager () = default;
    ~DagManager () = default;

    void            addNode(const RelationTypePtr &node);
    void            deleteNode(const RelationTypePtr &node);

    void            addEdge(const RelationTypePtr &nodeA, const RelationTypePtr &nodeB);
    void            deleteEdge(const RelationTypePtr &nodeA, const RelationTypePtr &nodeB);

  private:
    using Graph     = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>;
    using Edge      = std::pair<boost::uuids::uuid, boost::uuids::uuid>;
    using Vertex    = boost::graph_traits<Graph>::vertex_descriptor;
    using VertexMap = std::map<boost::uuids::uuid, Vertex>;

    Graph           m_graph;
    VertexMap       m_vertices;

    struct cycleDetector : public boost::dfs_visitor<>
    {
        cycleDetector(bool& has_cycle) : m_has_cycle(has_cycle) { }

        template <class Edge, class Graph>
        void back_edge(Edge, Graph&) { m_has_cycle = true; }

      protected:
        bool& m_has_cycle;
    };
};

}

#endif
