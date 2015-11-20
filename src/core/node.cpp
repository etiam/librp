#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sstream>
#include <string>
#include <sstream>
#include <ut/exempt_ptr.h>

#include "exception.h"
#include "logging.h"
#include "eventManager.h"
#include "nodeManager.h"
#include "node.h"

namespace Sg
{

Node::Node()
{
    m_type = UNMANGLE(*this);
}

Node::~Node()
{
}

const std::string &
Node::name() const
{
    return std::get<0>(*m_treeIterator);
}

std::string
Node::fullName() const
{
    return Master::nodeManager().fullName(m_treeIterator);
}

void
Node::addAttribute(const std::string &attrname, std::unique_ptr<Attribute> attribute)
{
    attribute->setName(attrname);

    // save temporary AttributePtr for later "attrsCreated" signal
    AttributePtr attrptr = attribute.get();

    Object::addAttribute(attrname, std::move(attribute));

    // have to emit here because the signal expects a NodePtr as parent, not an ObjectPtr.
    Master::eventManager().emit<NodePtr, AttributePtr>("attrsCreated", NodePtr(this), attrptr);
}

void
Node::delAttribute(const std::string &attrname)
{
    Object::delAttribute(attrname);

    // have to emit here because the signal expects a NodePtr as parent, not an ObjectPtr.
    Master::eventManager().emit<NodePtr, std::string>("attrsDeleted", NodePtr(this), attrname);
}

void
Node::setAttribute(const std::string &attrname, const AttributePtr &attrptr)
{
    Object::setAttribute(attrname, attrptr);

    // have to emit here because the signal expects a NodePtr as parent, not an ObjectPtr.
    Master::eventManager().emit<NodePtr, AttributePtr>("attrsChanged", NodePtr(this), attrptr);
}

void
Node::initializeNode()
{
}

void
Node::emitChanged(const AttributePtr &attribute) const
{
    Master::eventManager().emit<NodePtr, AttributePtr>("attrsChanged", NodePtr(const_cast<Node*>(this)), attribute);
}

}
