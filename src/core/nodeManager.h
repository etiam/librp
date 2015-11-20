#pragma once
#ifndef SGNODEMGR_H
#define SGNODEMGR_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>
#include <vector>
#include <set>
#include <mutex>
#include <boost/utility.hpp>

#include "classPointers.h"
#include "node.h"
#include "nameTree.h"

namespace Sg
{

using NodeList = std::vector<NodePtr>;

class NodeManager : boost::noncopyable
{
  public:
    NodeManager() = default;
    ~NodeManager() = default;

    std::string                             fullName(const NameTree::Iterator &node) const;

    std::string                             uniqueName(std::string name);

    bool                                    exists(const std::string &name) const;

    NodePtr                                 findNode(const std::string &name);

    template <typename T>
    std::exempt_ptr<T>                      findNode(const std::string &name);

    NodeList                                findNodes(const std::string& pattern);

    template <typename T>
    std::vector<std::exempt_ptr<T>>         findNodes(const std::string &pattern);

    const NodeList &                        nodes() const;

    template <typename T>
    std::vector<std::exempt_ptr<T>>         nodes() const;

    const NodePtr &                         addNode(const std::string &path, std::unique_ptr<Node> node);

    void                                    deleteNode(const NodePtr &node);

    NameTree::Iterator                      renameNode(const std::string &from, const std::string &to);

    size_t                                  nodeCount() const;

    void                                    lock() const;
    void                                    unlock() const;

  private:
    std::vector<std::unique_ptr<Node>>      m_nodes;
    NodeList                                m_refnodes;
    NameTree                                m_nameTree;
    mutable std::mutex                      m_lock;
};

template <typename T>
std::exempt_ptr<T>
NodeManager::findNode(const std::string &name)
{
    return std::exempt_pointer_cast<T>(findNode(name));
}

template <typename T>
std::vector<std::exempt_ptr<T>>
NodeManager::findNodes(const std::string &pattern)
{
    std::vector<std::exempt_ptr<T>> nodes;

    for (const auto &node : findNodes(pattern))
    {
        if (auto p = dynamic_cast<T *>(node.get()))
            nodes.push_back(p);
    }

    return nodes;
}

template <typename T>
std::vector<std::exempt_ptr<T>>
NodeManager::nodes() const
{
    std::vector<std::exempt_ptr<T>> nodes;

    m_lock.lock();

    for (const auto &node : m_refnodes)
    {
        if (auto p = dynamic_cast<T *>(node.get()))
            nodes.push_back(p);
    }

    m_lock.unlock();

    return nodes;
}

}

#endif
