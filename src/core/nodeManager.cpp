#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>

#include "global.h"
#include "logging.h"
#include "node.h"
#include "nodeManager.h"
#include "relationshipManager.h"
#include "dagManager.h"

namespace Sg
{

std::string
NodeManager::uniqueName(std::string name)
{
    return m_nameTree.uniqueName(name);
}

std::string
NodeManager::fullName(const NameTree::Iterator &node) const
{
    m_lock.lock();
    auto fullname = m_nameTree.fullName(node);
    m_lock.unlock();

    return fullname;
}

bool
NodeManager::exists(const std::string &name) const
{
    return m_nameTree.findName(name) != m_nameTree.end();
}

NodePtr
NodeManager::findNode(const std::string &name)
{
    NodePtr result = nullptr;

    auto node = m_nameTree.findName(name);
    if (node != m_nameTree.end())
        result = std::get<1>(*node).get();

    return result;
}

NodeList
NodeManager::findNodes(const std::string& pattern)
{
    NodeList nodes;

    auto treenodes = m_nameTree.findNames(pattern);
    nodes.reserve(treenodes.size());

    std::for_each(std::begin(treenodes), std::end(treenodes),
                  [&](const NameTree::Iterator &treenode) {nodes.push_back(std::get<1>(*treenode));});

    return nodes;
}

const NodeList &
NodeManager::nodes() const
{
    return m_refnodes;
}


const NodePtr &
NodeManager::addNode(const std::string &path, std::unique_ptr<Node> node)
{
    assert(node != nullptr);

    m_lock.lock();

    auto ref = std::exempt_ptr<Node>(node.get());

    // add node name to name tree and store iterator to tree location in node
    node->m_treeIterator = m_nameTree.addName(m_nameTree.uniqueName(path), ref);

    // store node
    m_refnodes.push_back(ref);
    m_nodes.push_back(std::move(node));

    const auto &refnode = m_refnodes.at(m_refnodes.size()-1);

    m_lock.unlock();

    return refnode;
}

void
NodeManager::deleteNode(const NodePtr &node)
{
    assert(node != nullptr);

    // store name tree iterator for deleting after everything else
    auto nameiter = node->m_treeIterator;

    // delete from m_refnodes
    m_lock.lock();

    auto rit = std::find_if(std::begin(m_refnodes), std::end(m_refnodes),
                           [&](const NodePtr &it) -> bool { return it.get() == node.get(); });
    if (rit != std::end(m_refnodes))
    {
        m_refnodes.erase(rit);
    }

    m_lock.unlock();


    // delete from m_nodes
    m_lock.lock();

    const auto &end = std::end(m_nodes);
    auto nit = std::find_if(std::begin(m_nodes), end,
                           [&](const std::unique_ptr<Node> &it) -> bool { return it.get() == node.get(); });
    auto found = nit != std::end(m_nodes);

    m_lock.unlock();

    if (found)
    {
        LSGC0 << "Sg::NodeManager::deleteNode(" << node->fullName() << ")";

        // delete node from any/all referencing relationships
        // TODO : use makeFalse() for this
        auto &rels = relationshipManager().relationships();
        auto relcmp = [&nit](const std::unique_ptr<Relationship> &rel) -> bool
                { return rel->subject().get() == nit->get() || rel->object().get() == nit->get(); };
        rels.erase(std::remove_if(rels.begin(), rels.end(), relcmp), rels.end());

        // delete node
        // since vector::erase() deletes the object before removing it from the vector and an object can potentially delete
        // another object in it's destructor, the vector can be changed before the erase() happens.  the solution is to
        // move it out of the vector first, then erase() it, then delete the object.
        m_lock.lock();

        auto temp = std::move(*nit);
        m_nodes.erase(nit);

        m_lock.unlock();

        temp.reset();
    }

    // delete node from name tree
    m_nameTree.deleteName(nameiter);
}

NameTree::Iterator
NodeManager::renameNode(const std::string &from, const std::string &to)
{
    return m_nameTree.renameName(from, m_nameTree.uniqueName(to));
}

size_t
NodeManager::nodeCount() const
{
    size_t count;

    m_lock.lock();

    count = m_nodes.size();

    m_lock.unlock();

    return count;
}

void
NodeManager::lock() const
{
    m_lock.lock();
}

void
NodeManager::unlock() const
{
    m_lock.unlock();
}

}
