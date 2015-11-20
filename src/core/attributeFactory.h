/*
 * attributeFactory.h
 *
 *  Created on: Jun 27, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGATTRIBUTEFACTORY_H
#define SGATTRIBUTEFACTORY_H

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

class AttributeFactory : boost::noncopyable
{
  public:
    AttributeFactory() = default;
    ~AttributeFactory() = default;

    template<typename ...Args>
    void                                    registerCreator(const std::string &type, const std::function<std::unique_ptr<Attribute> (Args...)> &functor);

    template<typename ...Args>
    void                                    registerCreator(const std::string &type, std::unique_ptr<Attribute> functor(Args...));

    template<typename Functor, typename ...Args>
    void                                    registerCreator(const std::string &type, const Functor &functor);

    template<typename ...Args>
    std::unique_ptr<Attribute>              create(const std::string &type, Args&&... args);

  private:
    std::map<std::string, generic::any>     m_creatorStore;
    std::map<std::string, generic::any>     m_creatorMap;
    std::map<std::string, std::string>      m_typeinfoMap;
};

template<typename ...Args>
void
AttributeFactory::registerCreator(const std::string &type, const std::function<std::unique_ptr<Attribute> (Args...)> &functor)
{
    using Functor = std::function<std::unique_ptr<Attribute> (Args...)>;

    // only allow single instance of type
    auto it = m_creatorStore.find(type);
    if (it != m_creatorStore.end())
        THROW (KeyExc, "Sg::AttributeFactory::registerCreator(): Could not register creator of already existing type \"" << type << "\".");

    // create creator
    auto creator = std::make_unique<Functor>(functor);

    // store reference to it
    m_creatorMap[type] = creator.get();

    // store it
    m_creatorStore[type] = std::move(creator);

    // used for error msg on incorrect function signature
    m_typeinfoMap[type] = UNMANGLE(std::unique_ptr<Attribute> (Args...));

    LSGN2 << "Sg::AttributeFactory::registerCreator(\"" << type << "\", " << UNMANGLE(functor) << ")";
}

template<typename ...Args>
void
AttributeFactory::registerCreator(const std::string &type, std::unique_ptr<Attribute> functor(Args...))
{
    registerCreator(type, std::function<std::unique_ptr<Attribute> (Args...)>(functor));
}


template<typename Functor, typename ...Args>
void
AttributeFactory::registerCreator(const std::string &type, const Functor &functor)
{
    registerCreator(type, std::function<std::unique_ptr<Attribute> (Args...)>(functor));
}

template<typename ...Args>
std::unique_ptr<Attribute>
AttributeFactory::create(const std::string &type, Args&&... args)
{
    using Functor = std::function<std::unique_ptr<Attribute> (Args...)>;

    auto it = m_creatorMap.find(type);
    if (it == m_creatorMap.end())
        THROW (KeyExc, "Sg::AttributeFactory::create(): Could not create attribute of unknown type \"" << type  << "\".");

    const auto creator = generic::any_cast<Functor *>(&(it->second));
    if (!creator)
        THROW (TypeExc, "Sg::AttributeFactory::create(): Could not create attribute of type \"" << type  <<
                        "\", args do not match creator signature, got \"" << UNMANGLE(std::unique_ptr<Attribute>(Args...)) << "\", expected \"" << m_typeinfoMap[type] << "\"");

    return (**creator)(std::forward<Args>(args)...);
}

}

#endif
