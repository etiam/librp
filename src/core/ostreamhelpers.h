/*
 * ostreamhelpers.h
 *
 *  Created on: Mar 23, 2012
 *      Author: jasonr
 */

#pragma once
#ifndef SGOSTREAMHELPERS_H_
#define SGOSTREAMHELPERS_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <typeinfo>
#include <ut/exempt_ptr.h>

#include "classPointers.h"
#include "attribute.h"

namespace Sg
{

class Relation;
class Relationship;
class RelationshipType;
class RelationshipManager;
class Timestamp;

std::ostream &operator <<(std::ostream &out, const Node &node);
std::ostream &operator <<(std::ostream &out, const Attribute &attr);
std::ostream &operator <<(std::ostream &out, const Relationship &rel);
std::ostream &operator <<(std::ostream &out, const Timestamp &ts);

} // namespace Sg

namespace std
{

template <typename T>
ostream &
operator <<(ostream &out, const exempt_ptr<T> &t)
{
//    return out << "exempt_ptr(" << &t << ", value=" << *(t.get()) << "" << ", p=" << t.get() << ")";
    return out << *t.get();
}

template <typename T, typename D>
ostream &
operator <<(ostream &out, const unique_ptr<T, D> &t)
{
//    return out << "unique_ptr(" << &t << ", value=" << *(t.get()) << "" << ", p=" << t.get() << ")";
    return out << *t.get();
}

} // namespace std

#endif // OSTREAMHELPERS_H_
