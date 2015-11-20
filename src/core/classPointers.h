/*
 * classPointers.h
 *
 *  Created on: Jan 8, 2012
 *      Author: jasonr
 */
#pragma once
#ifndef SGCLASSPOINTERS_H_
#define SGCLASSPOINTERS_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <memory>
#include <ut/exempt_ptr.h>

namespace Sg
{

class Node;
class Attribute;
class Driver;
class Relation;
class Relationship;
class RelationshipType;

using DriverPtr                 = std::exempt_ptr<Driver>;
using NodePtr                   = std::exempt_ptr<Node>;
using AttributePtr              = std::exempt_ptr<Attribute>;
using RelationTypePtr           = std::exempt_ptr<Relation>;
using RelationshipPtr           = std::exempt_ptr<Relationship>;
using RelationshipTypePtr       = std::exempt_ptr<RelationshipType>;

}

#endif // SGCLASSPOINTERS_H_
