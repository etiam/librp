#pragma once
#ifndef SGDRIVER_H
#define SGDRIVER_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "classPointers.h"

namespace Sg 
{

/** Abstract base class for objects which can be used to drive the value 
of an attribute. */

class Driver 
{
  public:
    virtual ~Driver();

  protected:
    /** Protected Constructor */
    Driver() = default;
};

}

#endif
