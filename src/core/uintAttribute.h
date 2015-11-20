/*
 * uintAttribute.h
 *
 *  Created on: Aug 2, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGUINTATTRIBUTE_H
#define SGUINTATTRIBUTE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <ut/exempt_ptr.h>

#include "classPointers.h"
#include "global.h"
#include "typedAttribute.h"

namespace Sg
{

template <typename... Args>
using UIntAttribute        = TypedAttribute<unsigned int, Args...>;

template <typename... Args>
using UIntAttributePtr     = std::exempt_ptr<TypedAttribute<unsigned int, Args...>>;

template <typename... Args>
using UIntFunctor          = Functor<unsigned int, Args...>;

template <typename... Args>
using UIntFunctorPtr       = typename FunctorPtr<unsigned int, Args...>::Type;

template <typename... Args>
using UIntLambdaFunctor    = LambdaFunctor<unsigned int, Args...>;

template <typename... Args>
using UIntLambdaFunctorPtr = typename LambdaFunctorPtr<unsigned int, Args...>::Type;


template <typename... Args>
class TypedAttribute<unsigned int, Args...> : public TypedAttributeBase<unsigned int, TypedAttribute<unsigned int, Args...>, Args...>
{
  private:
   using Super = TypedAttributeBase<unsigned int, TypedAttribute<unsigned int, Args...>, Args...>;

  public:
    TypedAttribute<unsigned int, Args...>();
    TypedAttribute<unsigned int, Args...>(const unsigned int &value);
    TypedAttribute<unsigned int, Args...>(unsigned int &&value);

    virtual std::string                 toString() const;
};


template <typename... Args>
TypedAttribute<unsigned int, Args...>::TypedAttribute() :
    Super("unsigned int", 0)
{
}

template <typename... Args>
TypedAttribute<unsigned int, Args...>::TypedAttribute(const unsigned int &value) :
    Super("unsigned int", value)
{
}

template <typename... Args>
TypedAttribute<unsigned int, Args...>::TypedAttribute(unsigned int &&value) :
    Super("unsigned int", std::move(value))
{
}

template <typename... Args>
std::string
TypedAttribute<unsigned int, Args...>::toString() const
{
    std::stringstream out;
    out << std::fixed << Super::m_value;
    return out.str();
}

template <typename... Args>
bool
isUIntAttribute(const AttributePtr &attr)
{
    return isTypedAttribute<unsigned int, Args...>(attr);
}

template <typename... Args>
UIntAttributePtr<Args...>
asUIntAttribute(const AttributePtr &attr)
{
    return asTypedAttribute<unsigned int, Args...>(attr);
};

template <typename... Args>
std::unique_ptr<TypedAttribute<unsigned int, Args...>>
asUIntAttribute(std::unique_ptr<Attribute> attr)
{
    return asTypedAttribute<unsigned int, Args...>(std::move(attr));
};

}

#endif
