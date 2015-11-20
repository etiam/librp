#pragma once
#ifndef SGEXCEPTION_H
#define SGEXCEPTION_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Iex.h>

namespace Sg
{
    DEFINE_EXC (SgExc,      Iex::BaseExc)  // Base exception
    DEFINE_EXC (SyntaxExc,  SgExc)
    DEFINE_EXC (TypeExc,    SgExc)
    DEFINE_EXC (ArgExc,     SgExc)
    DEFINE_EXC (IndexExc,   SgExc)
    DEFINE_EXC (KeyExc,     SgExc)
    DEFINE_EXC (NullExc,    SgExc)
    DEFINE_EXC (ValueExc,   SgExc)
    DEFINE_EXC (IOExc,      SgExc)
    DEFINE_EXC (UnknownExc, SgExc)         // Unknown error
}

#endif
