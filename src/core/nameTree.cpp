/*
 * nameTree.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <ut/utils.h>

#include "exception.h"
#include "tree_util.hh"
#include "nameTree.h"

namespace Sg
{

NameTree::NameTree() :
    m_root(m_tree.set_head(std::make_tuple("/", nullptr)))
{
}

std::string
NameTree::uniqueName(std::string path) const
{
    static boost::regex pattern(R"regex(/[_a-zA-Z][\./\-_a-zA-Z0-9]*)regex");
    std::string result;

    // must start with '/'
    if (path[0] != '/')
        THROW (SyntaxExc, "NameTree::uniqueName(): name must start with '/' character in \"" << path << "\".");

    // check for invalid characters
    boost::smatch what;
    if (!boost::regex_match(path, what, pattern))
        THROW (SyntaxExc, "NameTree::uniqueName(): invalid characters in \"" << path << "\".");

    // check if branch exists, add it if not
    auto temp = Ut::splitPath(path);
    auto branch = temp[0];
    auto name = temp[1];

    // handle duplicate name by appending with a unique version number
    if (findName(path) != end())
    {
        // get the base of path and numeric extension (if it exists)
        std::string base;
        uint version;
        std::tie(base, version) = splitName(path);

        // start new unique versions at "1" not "0"
        if (version == 0)
            version = 1;

        m_lock.lock();

        // use the highest extension + 1 from the m_versionMap
        if (m_versionMap.find(base) != std::end(m_versionMap))
        {
            const auto &baseversions = m_versionMap.at(base);
            if (baseversions.find(version) != std::end(baseversions))
                version = *baseversions.rbegin() + 1;
        }

        m_lock.unlock();

        result = base + std::to_string(static_cast<unsigned long long>(version));
    }
    else
    {
        result = path;
    }

    return result;
}

std::string
NameTree::fullName(const Iterator &node) const
{
    std::stringstream str;

    // recursively append node name to str until we're at the root node
    if (m_tree.depth(node) != 0)
    {
        str << fullName(m_tree.parent(node));
        if (m_tree.depth(node) > 1)
            str << "/";
    }
    str << std::get<0>(*node);

    return str.str();
}

NameTree::Iterator
NameTree::findName(const std::string& path) const
{
    m_lock.lock();

    auto result = m_tree.end();

    auto node = m_names.find(path);

    if (node != std::end(m_names))
        result = node->second;

    m_lock.unlock();

    return result;
}

std::vector<NameTree::Iterator>
NameTree::findNames(const std::string& pattern) const
{
    auto cpattern = pattern.c_str();
    std::vector<Iterator> results;

    m_lock.lock();

    std::vector<std::string> names;
    for (const auto &n : m_names)
    {
        if (m_tree.depth(n.second) > 0 && Ut::wildCompare(cpattern, n.first.c_str()))
        {
            if (Ut::wildCompare(cpattern, n.first.c_str()))
                results.push_back(n.second);
        }
    }

    m_lock.unlock();

    return results;
}

NameTree::Iterator
NameTree::addName(const std::string& path, const NodePtr &node)
{
    auto newName = m_root;
    std::string baseName;

    // find parent branch insertion node
    // add non existing branches as we search
    for (auto name : Ut::splitString(path, "/"))
    {
        if (!name.size())
            continue;
        baseName += "/" + name;
        auto it = findName(baseName);
        if (it == end())
        {
            m_lock.lock();

            newName = m_tree.append_child(newName, std::make_tuple(name, node));

            // store fullname with associated iterator
            m_names[fullName(newName)] = newName;

            m_lock.unlock();
        }
        else
        {
            newName = it;
        }
    }

    m_lock.lock();

    // store fullname with associated iterator
    m_names[fullName(newName)] = newName;

    // store the base and version for future calls to uniqueName().
    std::string base;
    uint version;
    std::tie(base, version) = splitName(path);
    if (version > 0)
        m_versionMap[base].insert(version);

    m_lock.unlock();

    return newName;
}

void
NameTree::deleteName(const std::string& path)
{
    auto node = findName(path);

    if (node != end())
        deleteName(node);
    else
        THROW (SyntaxExc, "NameTree::deleteNode(): \"" << path << "\" not found.");
}

void
NameTree::deleteName(const Iterator& node, bool rebuild)
{
    m_lock.lock();

    if (m_tree.depth(node) < 1)
        THROW (Sg::SyntaxExc, "NameTree::deleteNode(): cannot delete root node.");
    if (m_tree.number_of_children(node) > 0)
        THROW (Sg::SyntaxExc, "NameTree::deleteNode(): cannot delete non empty branch \"" << fullName(node) << "\".");

    m_names.erase(fullName(node));
    m_tree.erase(node);

    m_lock.unlock();

//    if (rebuild)
//        rebuildMaps();
}

void
NameTree::deleteNames(const std::string& pattern)
{
    // nodes must be deleted in highest to lowest depth order
    auto nodes = findNames(pattern);

    m_lock.lock();
    std::sort(std::begin(nodes), std::end(nodes), [=](const Iterator &a, const Iterator &b){ return m_tree.depth(a) > m_tree.depth(b);});
    m_lock.unlock();

    for (const auto &node : nodes)
        deleteName(node, false);

//    rebuildMaps();
}

NameTree::Iterator
NameTree::renameName(const std::string &from, const std::string &to)
{
    Iterator fromnode;

    if (from != to)
    {
        if (from == "/")
            THROW (Sg::SyntaxExc, "NameTree::renameNode(): cannot rename root node.");

        if (Ut::splitPath(from)[0] != Ut::splitPath(to)[0])
            THROW (Sg::SyntaxExc, "NameTree::renameNode(): cannot move node, only leaf node name changes allowed.");

        fromnode = findName(from);
        if (fromnode == end())
            THROW (SyntaxExc, "NameTree::deleteNode(): \"" << from << "\" not found.");

        m_lock.lock();
        std::get<0>(*fromnode) = Ut::splitPath(to)[1];
        m_lock.unlock();

        rebuildMaps();
    }

    return fromnode;
}

NameTree::Iterator
NameTree::end() const
{
    m_lock.lock();
    auto end = m_tree.end();
    m_lock.unlock();

    return end;
}

void
NameTree::rebuildMaps()
{
    m_lock.lock();

    m_names.clear();
    m_versionMap.clear();

    for (auto child = m_tree.begin_breadth_first(); child != m_tree.end_breadth_first(); ++child)
    {
        auto fullname = fullName(child);

        if (m_tree.depth(child) > 0)
            m_names[fullname] = child;

        std::string base;
        uint version;
        std::tie(base, version) = splitName(fullname);
        if (child.number_of_children() == 0 && version > 0)
            m_versionMap[base].insert(version);
    }

    m_lock.unlock();
}

std::tuple<std::string, uint>
NameTree::splitName(const std::string &path) const
{
    std::string base = path;
    uint version = 0;

    for (auto i=path.length()-1; i > 0; --i)
    {
        if (!std::isdigit(path[i]))
        {
            if (i+1 < path.length())
            {
                base = path.substr(0, i+1);
                auto temp = path.substr(i+1, path.length() - i);

                try
                {
                    version = boost::lexical_cast<uint>(temp);
                }
                catch(const boost::bad_lexical_cast &)
                {
                }
            }

            break;
        }
    }

    return std::make_tuple(base, version);
}

std::string
NameTree::dumpTree() const
{
    std::stringstream str;

    kptree::print_subtree_bracketed(m_tree, m_root, str);
    return str.str();
}

std::string
NameTree::dumpNames() const
{
    std::stringstream str;

    for (const auto &n : m_names)
        str << "'" << n.first << "', ";
    return "[" + str.str().substr(0, str.str().size()-2) + "]";
}

std::string
NameTree::dumpNameMap() const
{
    std::stringstream str;

    for (const auto &n : m_versionMap)
        str << n.first << " : " << Ut::dumpSet(n.second) << std::endl;

    return str.str();
}

std::ostream &
operator <<(std::ostream &out, const NameTree::TreeNode &node)
{
    return (out << std::get<0>(node));
}

std::ostream &  operator <<(std::ostream &out, const NameTree::Iterator &iterator)
{
    return (out << "\"iterator for '" << *iterator << "'\"");
}

} // namespace Sg
