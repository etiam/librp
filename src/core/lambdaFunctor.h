#pragma once
#ifndef SGLAMBDAFUNCTOR_H
#define SGLAMBDAFUNCTOR_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <memory>

#include "classPointers.h"
#include "driver.h"
#include "timestamp.h"
#include "exception.h"

namespace Sg
{

template <typename T, typename... Args>
class LambdaFunctor : public Driver
{
  public:
    LambdaFunctor(std::function<T (Args...)> functor);

    const T &                       value(const Args... args) const;

    const Timestamp &               timestamp();

  private:
    mutable Timestamp               m_timestamp;
    mutable T                       m_value;
    std::function<T (Args...)>      m_functor;
};

template <typename T, typename... Args>
struct LambdaFunctorPtr
{
    using Type = std::exempt_ptr<LambdaFunctor<T, Args...>>;
};

template <typename T, typename... Args>
LambdaFunctor<T, Args...>::LambdaFunctor(std::function<T (Args...)> functor) :
    m_functor(functor)
{
}

template <class T, typename... Args>
const T &
LambdaFunctor<T, Args...>::value(const Args... args) const
{
    m_value = m_functor(args...);
    m_timestamp.set();
    return m_value;
}

template <typename T, typename... Args>
const Timestamp &
LambdaFunctor<T, Args...>::timestamp()
{
    return m_timestamp;
}

}

#endif
