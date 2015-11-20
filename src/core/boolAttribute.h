/*
 * boolAttribute.h
 *
 *  Created on: Aug 2, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGBOOLATTRIBUTE_H
#define SGBOOLATTRIBUTE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sstream>

#include "classPointers.h"
#include "global.h"
#include "typedAttribute.h"

namespace Sg
{

template <typename... Args>
using BoolAttribute        = TypedAttribute<bool, Args...>;

template <typename... Args>
using BoolAttributePtr     = std::exempt_ptr<TypedAttribute<bool, Args...>>;

template <typename... Args>
using BoolFunctor          = Functor<bool, Args...>;

template <typename... Args>
using BoolFunctorPtr       = typename FunctorPtr<bool, Args...>::Type;

template <typename... Args>
using BoolLambdaFunctor    = LambdaFunctor<bool, Args...>;

template <typename... Args>
using BoolLambdaFunctorPtr = typename LambdaFunctorPtr<bool, Args...>::Type;


template <typename... Args>
class TypedAttribute<bool, Args...> : public TypedAttributeBase<bool, TypedAttribute<bool, Args...>, Args...>
{
  private:
   using Super = TypedAttributeBase<bool, TypedAttribute<bool, Args...>, Args...>;

  public:
    TypedAttribute<bool, Args...>();
    TypedAttribute<bool, Args...>(const bool &value);
    TypedAttribute<bool, Args...>(bool &&value);
    ~TypedAttribute<bool, Args...>() {};

    virtual std::string                 toString() const;
};


template <typename... Args>
TypedAttribute<bool, Args...>::TypedAttribute() :
    Super("bool", bool())
{
}

template <typename... Args>
TypedAttribute<bool, Args...>::TypedAttribute(const bool &value) :
    Super("bool", value)
{
}

template <typename... Args>
TypedAttribute<bool, Args...>::TypedAttribute(bool &&value) :
    Super("bool", std::move(value))
{
}

template <typename... Args>
std::string
TypedAttribute<bool, Args...>::toString() const
{
    std::stringstream out;
    out << (Super::m_value ? "true" : "false");
    return out.str();
}

template <typename... Args>
bool
isBoolAttribute(const AttributePtr &attr)
{
    return isTypedAttribute<bool, Args...>(attr);
}

template <typename... Args>
BoolAttributePtr<Args...>
asBoolAttribute(const AttributePtr &attr)
{
    return asTypedAttribute<bool, Args...>(attr);
};

template <typename... Args>
std::unique_ptr<TypedAttribute<bool, Args...>>
asBoolAttribute(std::unique_ptr<Attribute> attr)
{
    return asTypedAttribute<bool, Args...>(std::move(attr));
};

}

#endif
