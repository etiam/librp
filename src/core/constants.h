#pragma once
#ifndef SGCONSTANTS_H
#define SGCONSTANTS_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <float.h>
#include <limits.h>

namespace Sg 
{

const float STATIC  = FLT_MAX-1.0;
const float EPSILON = FLT_EPSILON;

}

#endif
