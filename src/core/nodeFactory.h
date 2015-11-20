/*
 * nodeFactory.h
 *
 *  Created on: Jun 27, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGNODEFACTORY_H
#define SGNODEFACTORY_H

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <functional>
#include <boost/utility.hpp>
#include <ut/make_unique.h>

#include "logging.h"
#include "genericany.h"
#include "classPointers.h"
#include "exception.h"

namespace Sg
{

class NodeFactory : boost::noncopyable
{
  public:
    NodeFactory() = default;
    ~NodeFactory() = default;

    template<typename ...Args>
    void                                    registerCreator(const std::string &type, const std::function<std::unique_ptr<Node> (Args...)> &functor);

    template<typename ...Args>
    void                                    registerCreator(const std::string &type, std::unique_ptr<Node> functor(Args...));

    template<typename Functor, typename ...Args>
    void                                    registerCreator(const std::string &type, const Functor &functor);

    template<typename ...Args>
    std::unique_ptr<Node>                   create(const std::string &type, Args&&... args);

  private:
    std::map<std::string, generic::any>     m_creatorStore;
    std::map<std::string, generic::any>     m_creatorMap;
    std::map<std::string, std::string>      m_typeinfoMap;
};

template<typename ...Args>
void
NodeFactory::registerCreator(const std::string &type, const std::function<std::unique_ptr<Node> (Args...)> &functor)
{
    using Functor = std::function<std::unique_ptr<Node> (Args...)>;

    // only allow single instance of type
    auto it = m_creatorStore.find(type);
    if (it != m_creatorStore.end())
        THROW (KeyExc, "Sg::NodeFactory::registerCreator(): Could not register creator of already existing type \"" << type << "\".");

    // create creator
    auto creator = std::make_unique<Functor>(functor);

    // store reference to it
    m_creatorMap[type] = creator.get();

    // store it
    m_creatorStore[type] = std::move(creator);

    // used for error msg on incorrect function signature
    m_typeinfoMap[type] = UNMANGLE(std::unique_ptr<Node> (Args...));

    LSGN2 << "Sg::NodeFactory::registerCreator(\"" << type << "\", " << UNMANGLE(functor) << ")";
}

template<typename ...Args>
void
NodeFactory::registerCreator(const std::string &type, std::unique_ptr<Node> functor(Args...))
{
    registerCreator(type, std::function<std::unique_ptr<Node> (Args...)>(functor));
}


template<typename Functor, typename ...Args>
void
NodeFactory::registerCreator(const std::string &type, const Functor &functor)
{
    registerCreator(type, std::function<std::unique_ptr<Node> (Args...)>(functor));
}

template<typename ...Args>
std::unique_ptr<Node>
NodeFactory::create(const std::string &type, Args&&... args)
{
    using Functor = std::function<std::unique_ptr<Node> (Args...)>;

    auto it = m_creatorMap.find(type);
    if (it == m_creatorMap.end())
        THROW (KeyExc, "Sg::NodeFactory::create(): Could not create node of unknown type \"" << type  << "\".");

    const auto creator = generic::any_cast<Functor *>(&(it->second));
    if (!creator)
        THROW (TypeExc, "Sg::NodeFactory::create(): Could not create node of type \"" << type  <<
                        "\", args do not match creator signature, got \"" << UNMANGLE(std::unique_ptr<Node> (Args...)) << "\", expected \"" << m_typeinfoMap[type] << "\"");

    return (**creator)(std::forward<Args>(args)...);
}

}

#endif
