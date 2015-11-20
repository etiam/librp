/*
 * floatAttribute.h
 *
 *  Created on: Aug 2, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGFLOATATTRIBUTE_H
#define SGFLOATATTRIBUTE_H

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
using FloatAttribute        = TypedAttribute<float, Args...>;

template <typename... Args>
using FloatAttributePtr     = std::exempt_ptr<TypedAttribute<float, Args...>>;

template <typename... Args>
using FloatFunctor          = Functor<float, Args...>;

template <typename... Args>
using FloatFunctorPtr       = typename FunctorPtr<float, Args...>::Type;

template <typename... Args>
using FloatLambdaFunctor    = LambdaFunctor<float, Args...>;

template <typename... Args>
using FloatLambdaFunctorPtr = typename LambdaFunctorPtr<float, Args...>::Type;


template <typename... Args>
class TypedAttribute<float, Args...> : public TypedAttributeBase<float, TypedAttribute<float, Args...>, Args...>
{
  private:
   using Super = TypedAttributeBase<float, TypedAttribute<float, Args...>, Args...>;

  public:
    TypedAttribute<float, Args...>();
    TypedAttribute<float, Args...>(const float &value);
    TypedAttribute<float, Args...>(float &&value);

    virtual std::string                 toString() const;
};


template <typename... Args>
TypedAttribute<float, Args...>::TypedAttribute() :
    Super("float", float())
{
}

template <typename... Args>
TypedAttribute<float, Args...>::TypedAttribute(const float &value) :
    Super("float", value)
{
}

template <typename... Args>
TypedAttribute<float, Args...>::TypedAttribute(float &&value) :
    Super("float", std::move(value))
{
}

template <typename... Args>
std::string
TypedAttribute<float, Args...>::toString() const
{
    std::stringstream out;
    out << std::fixed << Super::m_value;
    return out.str();
}

template <typename... Args>
bool
isFloatAttribute(const AttributePtr &attr)
{
    return isTypedAttribute<float, Args...>(attr);
}

template <typename... Args>
FloatAttributePtr<Args...>
asFloatAttribute(const AttributePtr &attr)
{
    return asTypedAttribute<float, Args...>(attr);
};

template <typename... Args>
std::unique_ptr<TypedAttribute<float, Args...>>
asFloatAttribute(std::unique_ptr<Attribute> attr)
{
    return asTypedAttribute<float, Args...>(std::move(attr));
};

}

#endif
