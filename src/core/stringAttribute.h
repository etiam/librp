/*
 * stringAttribute.h
 *
 *  Created on: Aug 2, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGSTRINGATTRIBUTE_H
#define SGSTRINGATTRIBUTE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>

#include "classPointers.h"
#include "global.h"
#include "typedAttribute.h"

namespace Sg
{

template <typename... Args>
using StringAttribute        = TypedAttribute<std::string, Args...>;

template <typename... Args>
using StringAttributePtr     = std::exempt_ptr<TypedAttribute<std::string, Args...>>;

template <typename... Args>
using StringFunctor          = Functor<std::string, Args...>;

template <typename... Args>
using StringFunctorPtr       = typename FunctorPtr<std::string, Args...>::Type;

template <typename... Args>
using StringLambdaFunctor    = LambdaFunctor<std::string, Args...>;

template <typename... Args>
using StringLambdaFunctorPtr = typename LambdaFunctorPtr<std::string, Args...>::Type;


template <typename... Args>
class TypedAttribute<std::string, Args...> : public TypedAttributeBase<std::string, TypedAttribute<std::string, Args...>, Args...>
{
  private:
   using Super = TypedAttributeBase<std::string, TypedAttribute<std::string, Args...>, Args...>;

  public:
    TypedAttribute<std::string, Args...>();
    TypedAttribute<std::string, Args...>(const std::string &value);
    TypedAttribute<std::string, Args...>(std::string &&value);

    virtual std::string                 toString() const;
};


template <typename... Args>
TypedAttribute<std::string, Args...>::TypedAttribute() :
    Super("string", std::string())
{
}

template <typename... Args>
TypedAttribute<std::string, Args...>::TypedAttribute(const std::string &value) :
    Super("string", value)
{
}

template <typename... Args>
TypedAttribute<std::string, Args...>::TypedAttribute(std::string &&value) :
    Super("string", std::move(value))
{
}

template <typename... Args>
std::string
TypedAttribute<std::string, Args...>::toString() const
{
    return Super::m_value;
}

template <typename... Args>
bool
isStringAttribute(const AttributePtr &attr)
{
    return isTypedAttribute<std::string, Args...>(attr);
}

template <typename... Args>
StringAttributePtr<Args...>
asStringAttribute(const AttributePtr &attr)
{
    return asTypedAttribute<std::string, Args...>(attr);
};

template <typename... Args>
std::unique_ptr<TypedAttribute<std::string, Args...>>
asStringAttribute(std::unique_ptr<Attribute> attr)
{
    return asTypedAttribute<std::string, Args...>(std::move(attr));
};

}

#endif
