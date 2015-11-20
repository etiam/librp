/*
 * intAttribute.h
 *
 *  Created on: Aug 2, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGINTATTRIBUTE_H
#define SGINTATTRIBUTE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "classPointers.h"
#include "global.h"
#include "typedAttribute.h"

namespace Sg
{

template <typename... Args>
using IntAttribute        = TypedAttribute<int, Args...>;

template <typename... Args>
using IntAttributePtr     = std::exempt_ptr<TypedAttribute<int, Args...>>;

template <typename... Args>
using IntFunctor          = Functor<int, Args...>;

template <typename... Args>
using IntFunctorPtr       = typename FunctorPtr<int, Args...>::Type;

template <typename... Args>
using IntLambdaFunctor    = LambdaFunctor<int, Args...>;

template <typename... Args>
using IntLambdaFunctorPtr = typename LambdaFunctorPtr<int, Args...>::Type;


template <typename... Args>
class TypedAttribute<int, Args...> : public TypedAttributeBase<int, TypedAttribute<int, Args...>, Args...>
{
  private:
   using Super = TypedAttributeBase<int, TypedAttribute<int, Args...>, Args...>;

  public:
    TypedAttribute<int, Args...>();
    TypedAttribute<int, Args...>(const int &value);
    TypedAttribute<int, Args...>(int &&value);

    virtual std::string                 toString() const;
};


template <typename... Args>
TypedAttribute<int, Args...>::TypedAttribute() :
    Super("int", int())
{
}

template <typename... Args>
TypedAttribute<int, Args...>::TypedAttribute(const int &value) :
    Super("int", value)
{
}

template <typename... Args>
TypedAttribute<int, Args...>::TypedAttribute(int &&value) :
    Super("int", std::move(value))
{
}

template <typename... Args>
std::string
TypedAttribute<int, Args...>::toString() const
{
    std::stringstream out;
    out << Super::m_value;
    return out.str();
}

template <typename... Args>
bool
isIntAttribute(const AttributePtr &attr)
{
    return isTypedAttribute<int, Args...>(attr);
}

template <typename... Args>
IntAttributePtr<Args...>
asIntAttribute(const AttributePtr &attr)
{
    return asTypedAttribute<int, Args...>(attr);
};

template <typename... Args>
std::unique_ptr<TypedAttribute<int, Args...>>
asIntAttribute(std::unique_ptr<Attribute> attr)
{
    return asTypedAttribute<int, Args...>(std::move(attr));
};

}

#endif
