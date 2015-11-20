/*
 * master.h
 *
 *  Created on: Mar 16, 2011
 *      Author: jasonr
 */

#pragma once
#ifndef SGMASTER_H
#define SGMASTER_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <boost/utility.hpp>
#include <string>

#include "classPointers.h"

namespace Sg
{

class NodeManager;
class EventManager;
class RelationshipManager;
class DagManager;
class NodeFactory;
class AttributeFactory;
class SelectionManager;

/** Singleton class containing various manager instances.
  * Master is a singleton class containing static member
  * functions which return references to manager instances.
  * The first time one of the static member functions is
  * called the singleton is initialized.
  */
class Master : boost::noncopyable
{
  public:
    ~Master() = default;

    /** Initialize module.  Sets up logging types, relationship types, etc. */
    static void                     		initialize();

    /** Delete module. */
    static void                             shutdown();

    /** Returns reference to nodeManager */
    static NodeManager &            		nodeManager(void);

    /** Returns reference to eventManager */
    static EventManager &           		eventManager(void);

    /** Returns reference to relationshipManager */
    static RelationshipManager &    		relationshipManager(void);

    /** Returns reference to dagManager */
    static DagManager &             		dagManager(void);

    /** Returns reference to nodeFactory */
    static NodeFactory &            		nodeFactory(void);

    /** Returns reference to attributeFactory */
    static AttributeFactory &               attributeFactory(void);

    /** Returns reference to selectionManager */
    static SelectionManager &          		selectionManager(void);

    /** Load node plugins found in system, envvar and optional path locations */
    static void                             loadPlugins(std::string path="");

  private:
    Master();

    static Master &                         instance();

    std::unique_ptr<NodeManager>            m_nodeManager;
    std::unique_ptr<EventManager>           m_eventManager;
    std::unique_ptr<RelationshipManager>    m_relationshipManager;
    std::unique_ptr<DagManager>             m_dagManager;
    std::unique_ptr<NodeFactory>            m_nodeFactory;
    std::unique_ptr<AttributeFactory>       m_attributeFactory;
    std::unique_ptr<SelectionManager>       m_selectionManager;
};

}

#endif
