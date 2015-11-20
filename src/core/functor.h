#pragma once
#ifndef SGFUNCTOR_H
#define SGFUNCTOR_H

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

template <class T, typename... Args>
class Functor : public Driver
{
  public:
    /** Constructor.
      * @param fp: pointer to function.
      */
    Functor(T (*fp)(const Args&... args));

    /** Constructor.
      * @param mfp: pointer to member function.
      * @param callee: reference to object containing member function.
      */
    template <class Callee, class CallType>
    Functor(T (CallType::* const &mfp)(const Args&... args), Callee *callee);

    /** Constructor.
      * @param mfp: pointer to member function.
      * @param callee: reference to object containing member function.
      */
    template <class Callee, class CallType>
    Functor(const T & (CallType::* const &mfp)(const Args&... args), Callee *callee);

    /** Constructor.
      * @param mfp: pointer to const member function.
      * @param callee: reference to object containing member function.
      */
    template <class Callee, class CallType>
    Functor(const T & (CallType::* const &mfp)(const Args&... args) const, Callee *callee);

    /** Constructor.
      * @param mfp: pointer to const member function.
      * @param callee: const reference to object containing member function.
      */
    template <class Callee, class CallType>
    Functor(T (CallType::* const &mfp)(const Args&... args) const, const Callee *callee);

    /** Return value of functor with variatic input.
      * @param time time at which to return value.
      * @return const reference to T
      */
    const T &                       value(const Args... args) const;

    /** Get the internal timestamp of this attribute */
    const Timestamp &               timestamp();

  private:
    mutable Timestamp               m_timestamp;
    mutable T                       m_value;
    std::function<T (Args...)>      m_functor;
};

template <class T, typename... Args>
struct FunctorPtr
{
    using Type = std::exempt_ptr<Functor<T, Args...>>;
};

template <class T, typename... Args>
Functor<T, Args...>::Functor(T (*fp)(const Args&... args))
{
    m_functor = fp;
}

template <class T, typename... Args> template <class Callee, class CallType>
Functor<T, Args...>::Functor(T (CallType::* const &mfp)(const Args&... args), Callee *callee)
{
    m_functor = std::bind(mfp, callee);
}

template <class T, typename... Args> template <class Callee, class CallType>
Functor<T, Args...>::Functor(const T & (CallType::* const &mfp)(const Args&... args), Callee *callee)
{
    m_functor = std::bind(mfp, callee);
}

template <class T, typename... Args> template <class Callee, class CallType>
Functor<T, Args...>::Functor(T (CallType::* const &mfp)(const Args&... args) const, const Callee *callee)
{
    m_functor = std::bind(mfp, callee);
}

template <class T, typename... Args>
const T &
Functor<T, Args...>::value(const Args... args) const
{
    m_value = m_functor(args...);
    m_timestamp.set();
    return m_value;
}

template <class T, typename... Args>
const Timestamp &
Functor<T, Args...>::timestamp()
{
    return m_timestamp;
}

}

#endif
