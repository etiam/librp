#pragma once
#ifndef SGNODE_H
#define SGNODE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <memory>
#include <string>

#include "classPointers.h"
#include "master.h"
#include "relation.h"
#include "typedAttribute.h"
#include "object.h"
#include "nameTree.h"

namespace Sg
{

/** Abstract Base Class for objects which can become nodes of a dag.
  *
  * Created objects are automatically added to the NodeManager/DagManager.
  */

class Node : public Relation,
             public Object
{
  public:
    virtual ~Node();

    const std::string &             name() const;

    std::string                     fullName() const;

    virtual std::string             nodeType() const = 0;

    virtual void                    addAttribute(const std::string &name, std::unique_ptr<Attribute> attribute);

    virtual void                    delAttribute(const std::string &name);

    virtual void                    setAttribute(const std::string &name, const AttributePtr &attrptr);

    template <typename T>
    void                            setAttribute(const std::string &name, T &value);

    template <typename T>
    void                            setAttribute(const std::string &name, const T &value);

    template <typename T>
    void                            setAttribute(const std::string &name, const TypedAttribute<T> &attribute);

    template <typename T>
    std::vector<std::exempt_ptr<TypedAttribute<T>>> attributesByType() const;

  protected:
    Node();

    virtual void                    initializeNode();

    friend NodePtr                  addNode(const std::string &path, std::unique_ptr<Node> node);

  private:
    /** Cannot access nodeManager() (needed for emit() call) within template functions defined here
      * but can within .cpp file.
      */
    void                            emitChanged(const AttributePtr &attr) const;

    // store iterator into nametree
    NameTree::Iterator              m_treeIterator;

    friend class                    NodeManager;
    friend bool                     operator ==(const Node &lhs, const Node &rhs);
    friend bool                     operator < (const Node &lhs, const Node &rhs);
};

template <typename T>
void
Node::setAttribute(const std::string &attrname, T &value)
{
    try
    {
        Object::setAttribute(attrname, value);
    }
    catch (SgExc &e)
    {
        REPLACE_EXC(e, "Node::setAttribute(" << fullName() << ") : " << e);
        throw;
    }

    // have to emit here because the signal expects a NodePtr as parent, not an ObjectPtr.
    emitChanged(attributeByName(attrname));
}

template <typename T>
void
Node::setAttribute(const std::string &attrname, const T &value)
{
    try
    {
        Object::setAttribute(attrname, value);
    }
    catch (SgExc &e)
    {
        REPLACE_EXC(e, "Node::setAttribute(" << fullName() << ") : " << e);
        throw;
    }

    // have to emit here because the signal expects a NodePtr as parent, not an ObjectPtr.
    emitChanged(attributeByName(attrname));
}

template <typename T>
void
Node::setAttribute(const std::string &attrname, const TypedAttribute<T> &attribute)
{
    try
    {
        Object::setAttribute(attrname, attribute);
    }
    catch (SgExc &e)
    {
        REPLACE_EXC(e, "Node::setAttribute(" << fullName() << ") : " << e);
        throw;
    }

    // have to emit here because the signal expects a NodePtr as parent, not an ObjectPtr.
    emitChanged(attributeByName(attrname));
}

template <typename T>
std::vector<std::exempt_ptr<TypedAttribute<T>>>
Node::attributesByType(void) const
{
    return Object::attributesByType<T>();
}

inline bool operator ==(const Node &lhs, const Node &rhs) { return lhs.name() == rhs.name(); }
inline bool operator !=(const Node &lhs, const Node &rhs) { return !operator==(lhs,rhs); }
inline bool operator < (const Node &lhs, const Node &rhs) { return lhs.name() < rhs.name(); }
inline bool operator > (const Node &lhs, const Node &rhs) { return  operator< (rhs,lhs); }
inline bool operator <=(const Node &lhs, const Node &rhs) { return !operator> (lhs,rhs); }
inline bool operator >=(const Node &lhs, const Node &rhs) { return !operator< (lhs,rhs); }

}

#endif
