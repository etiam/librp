/*
 * master.cpp
 *
 *  Created on: Mar 16, 2011
 *      Author: jasonr
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <cstring>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glob.h>
#include <dlfcn.h>
#include <ut/make_unique.h>
#include <ut/utils.h>

#include "logging.h"
#include "nodeFactory.h"
#include "attributeFactory.h"
#include "nodeManager.h"
#include "eventManager.h"
#include "dagManager.h"
#include "relationshipManager.h"
#include "selectionManager.h"
#include "boolAttribute.h"
#include "floatAttribute.h"
#include "intAttribute.h"
#include "uintAttribute.h"
#include "stringAttribute.h"
#include "master.h"

namespace Sg
{

std::unique_ptr<Master> theinstance = nullptr;

void
Master::initialize()
{
    // register logging channels
    Log::registerChannel("sg:core",   "sg cor");
    Log::registerChannel("sg:node",   "sg nod");
    Log::registerChannel("sg:events", "sg eve");

    // register default event types
    auto &eventManager = instance().m_eventManager;
    eventManager->registerEventType<NodePtr>("nodesCreated");
    eventManager->registerEventType<std::string>("nodesDeleted");
    eventManager->registerEventType<NodePtr>("nodeDeletionRequested", false);
    eventManager->registerEventType<NodePtr, std::string, std::string>("nodesRenamed");
    eventManager->registerEventType<NodePtr, AttributePtr>("attrsCreated");
    eventManager->registerEventType<NodePtr, AttributePtr>("attrsChanged");
    eventManager->registerEventType<NodePtr, std::string>("attrsDeleted");
    eventManager->registerEventType<NodePtr, std::string, NodePtr>("relationshipsAdded");
    eventManager->registerEventType<NodePtr, std::string, NodePtr>("relationshipsRemoved");
    eventManager->registerEventType("selectionChanged");

    auto &attrFactory = instance().m_attributeFactory;
    attrFactory->registerCreator("bool",    []() { return newAttribute<bool>(); });
    attrFactory->registerCreator("float",   []() { return newAttribute<float>(); });
    attrFactory->registerCreator("int",     []() { return newAttribute<int>(); });
    attrFactory->registerCreator("uint",    []() { return newAttribute<unsigned int>(); });
    attrFactory->registerCreator("string",  []() { return newAttribute<std::string>(); });
}

NodeManager &
Master::nodeManager()
{
    return *instance().m_nodeManager;
}

EventManager &
Master::eventManager()
{
    return *instance().m_eventManager;
}

RelationshipManager &
Master::relationshipManager()
{
    return *instance().m_relationshipManager;
}

DagManager &
Master::dagManager()
{
    return *instance().m_dagManager;
}

NodeFactory &
Master::nodeFactory()
{
    return *instance().m_nodeFactory;
}

AttributeFactory &
Master::attributeFactory()
{
    return *instance().m_attributeFactory;
}


SelectionManager &
Master::selectionManager()
{
    return *instance().m_selectionManager;
}

void
Master::loadPlugins(std::string path)
{
    void *                      handle;
    char *                      error;

    glob_t                      globbuf;
    std::string                 pattern;

    std::vector<std::string>    pathList;
    std::vector<std::string>    pluginList;

    LSGC0 << "Sg::Master::loadPlugins()";
    Log::incIndent();

    // start with passed in path
    if (path.length())
    {
        auto tokens = Ut::splitString(path, ":");
        std::copy(std::begin(tokens), std::end(tokens), std::back_inserter(pathList));
    }

    // then PLUGIN_PATH
    {
        auto tokens = Ut::splitString(PLUGIN_PATH, ":");
        std::copy(std::begin(tokens), std::end(tokens), std::back_inserter(pathList));
    }

    // and finally append path(s) in SG_PLUGIN_PATH envvar
    if (getenv("SG_PLUGIN_PATH"))
    {
        auto tokens = Ut::splitString(getenv("SG_PLUGIN_PATH"), ":");
        std::copy(std::begin(tokens), std::end(tokens), std::back_inserter(pathList));
    }

    LSGC2 << "pathList:";
    Log::incIndent();
    LSGC2 << Ut::dumpVector(pathList);
    Log::decIndent();

    // build list of *.so files in pathlist
    for (const auto &it : pathList)
    {
        // glob for plugins
        pattern = it + "/*.so";
        globbuf.gl_offs = 0;
        // TODO : check for error on glob() call
        glob(pattern.c_str(), GLOB_ERR,0, &globbuf);

        for(uint i=0; i < globbuf.gl_pathc; ++i)
            pluginList.push_back(globbuf.gl_pathv[i]);

        globfree(&globbuf);
    }

    // attempt to load found plugins
    for (const auto &it : pluginList)
    {
        LSGC1 << "loading " << it << "...";

        // try to open each shared-lib
        handle = dlopen(it.c_str(), RTLD_LAZY | RTLD_LOCAL);

        if(!handle)
        {
           std::cerr << "Sg::Master::loadPlugins() : cannot dlopen: " << dlerror() << std::endl;
           continue;
        }

        // load the symbols
        dlerror();
        auto registernode = (bool (*)()) dlsym(handle, "registerNode");
        if ((error = dlerror()) != NULL)
        {
            std::cerr << "Sg::Master::loadPlugins() : cannot load symbols: " << error << std::endl;
            dlclose(handle);
            continue;
        }

        // call plugin's registerNode function
        if (!registernode())
        {
            std::cerr << "Sg::Master::loadPlugins() : error registering plugin \\" << it << "\"."  << std::endl;
            dlclose(handle);
            continue;
        }

//        dlclose(handle);  // TODO : comment this out and segfault
    }

    Log::decIndent();
}

Master::Master() :
    m_nodeManager(std::make_unique<NodeManager>()),
    m_eventManager(std::make_unique<EventManager>()),
    m_relationshipManager(std::make_unique<RelationshipManager>()),
    m_dagManager(std::make_unique<DagManager>()),
    m_nodeFactory(std::make_unique<NodeFactory>()),
    m_attributeFactory(std::make_unique<AttributeFactory>()),
    m_selectionManager(std::make_unique<SelectionManager>())
{
}

Master &
Master::instance()
{
    if (!theinstance)
        theinstance = std::unique_ptr<Master>(new Master());
    return *theinstance;
}

void
Master::shutdown()
{
    if (theinstance)
    {
        // destroy managers in a specific order
        theinstance->m_nodeManager.reset();
        theinstance->m_relationshipManager.reset();
        theinstance->m_dagManager.reset();
        theinstance->m_nodeFactory.reset();
        theinstance->m_attributeFactory.reset();
        theinstance->m_selectionManager.reset();

        // make sure eventManager gets destroyed last because events need to be emitted
        // while other managers are being destroyed.
        theinstance->m_eventManager.reset();

        theinstance.reset();
    }
}

}
