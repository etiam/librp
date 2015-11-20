/*
 * nameTree.h
 *
 *  Created on: Nov 17, 2014
 *      Author: jasonr
 */

#ifndef NAMETREE_H_
#define NAMETREE_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sstream>
#include <vector>
#include <list>
#include <tuple>
#include <map>
#include <set>
#include <string>
#include <mutex>

#include "classPointers.h"
#include "tree.hh"

namespace Sg
{

class NameTree
{
  public:
    using TreeNode                        = std::tuple<std::string, NodePtr>;
    using Tree                            = tree<TreeNode>;
    using Iterator                        = Tree::iterator;

    NameTree();
    ~NameTree() = default;

    std::string                             uniqueName(std::string path) const;

    std::string                             fullName(const NameTree::Iterator &node) const;

    Iterator                                findName(const std::string& path) const;
    std::vector<Iterator>                   findNames(const std::string& pattern) const;

    Iterator                                addName(const std::string& path, const NodePtr &node);

    void                                    deleteName(const std::string& path);
    void                                    deleteName(const Iterator& node, bool rebuild=true);
    void                                    deleteNames(const std::string& pattern);

    Iterator                                renameName(const std::string &from, const std::string &to);

    Iterator                                end() const;

  private:
    void                                    rebuildMaps();

    std::tuple<std::string, uint>           splitName(const std::string &path) const;

  private:
    std::string                             dumpTree() const;
    std::string                             dumpNames() const;
    std::string                             dumpNameMap() const;

    friend std::ostream &                   operator <<(std::ostream &out, const TreeNode &node);
    friend std::ostream &                   operator <<(std::ostream &out, const Iterator &iterator);

    Tree                                    m_tree;
    Iterator                                m_root;

    std::map<std::string, Iterator>         m_names;
    std::map<std::string, std::set<uint>>   m_versionMap;

    mutable std::mutex                      m_lock;
};

std::ostream &  operator <<(std::ostream &out, const NameTree::TreeNode &node);
std::ostream &  operator <<(std::ostream &out, const NameTree::Iterator &iterator);

} // namespace Sg

#endif // NAMETREE_H_
