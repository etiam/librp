# Process this file with autoconf to produce a configure script.

## ------------------------ ##
## Autoconf requirements    ##
## ------------------------ ##
AC_PREREQ([2.69])

## ------------------------ ##
## Initialisation           ##
## ------------------------ ##

# Autoconf
AC_INIT([librp],[0.0.1],[rosson@gmail.com])
AC_CONFIG_SRCDIR([src/core/driver.h])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_SUBST(VERSION)

# Automake
AM_INIT_AUTOMAKE([1.11 foreign -Wgnu subdir-objects])
AM_MAINTAINER_MODE
AM_SANITY_CHECK
AM_PROG_LEX
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])

# Libtool 
LT_INIT
LIBRARY_VERSION=1:0:0
AC_SUBST(LIBRARY_VERSION)

## ----------------------- ##
## Checks for programs     ##
## ----------------------- ##
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_GREP
AC_PROG_SED
AC_PROG_YACC

PKG_PROG_PKG_CONFIG

## ----------------------- ##
## Checks for typedefs,    ##
## structures, and compiler##
## characteristics.        ##
## ----------------------- ##
AC_HEADER_STDC
AC_C_CONST
AC_C_INLINE
AC_HEADER_STDBOOL

## ----------------------- ##
## Checks for libraries    ##
## ----------------------- ##

# libut
PKG_CHECK_MODULES(LIBUT, libut >= 0.0.1)

# ilmbase
PKG_CHECK_MODULES(ILMBASE, IlmBase >= 2.1.0)

# boost
BOOST_REQUIRE(1.55)
BOOST_TEST
BOOST_IOSTREAMS

## ----------------------- ##
## Optional arguments      ##
## ----------------------- ##

# Evaluate shell variable equivalents to Makefile directory variables
if test "x$prefix" = xNONE
then
  prefix=$ac_default_prefix
fi
# Let make expand exec_prefix.
if test "x$exec_prefix" = xNONE
then
  exec_prefix='${prefix}'
fi
eval "eval LIB_DIR=$libdir"
AC_SUBST(LIB_DIR)

# remove autoconf's "-g -O2" defaults
CFLAGS=`echo $CFLAGS | sed 's/-O.//g'`
CFLAGS=`echo $CFLAGS | sed 's/-g.//g'`
CFLAGS=`echo $CFLAGS | sed 's/-O//g'`
CFLAGS=`echo $CFLAGS | sed 's/-g//g'`
CXXFLAGS=`echo $CXXFLAGS | sed 's/-O.//g'`
CXXFLAGS=`echo $CXXFLAGS | sed 's/-g.//g'`
CXXFLAGS=`echo $CXXFLAGS | sed 's/-O//g'`
CXXFLAGS=`echo $CXXFLAGS | sed 's/-g//g'`

CFLAGS="${CFLAGS} -fno-strict-aliasing -Wall -Werror"
CXXFLAGS="${CXXFLAGS} -fno-strict-aliasing -Wall -Werror -std=c++11"

# --enable-debug
AC_ARG_ENABLE([debug],
  [  --enable-debug    turn on compiler debugging],
  [case "${enableval}" in
    yes) debug=true ;;
    no)  debug=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
  esac],[debug=false])
if test "x$debug" != xfalse; then
      CFLAGS="-g3 -fno-inline ${CFLAGS}"
      CXXFLAGS="-g3 -fno-inline ${CXXFLAGS}"
fi

# --enable-opt
AC_ARG_ENABLE([opt],
  [  --enable-opt    turn on compiler optging],
  [case "${enableval}" in
    yes) opt=true ;;
    no)  opt=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-opt]) ;;
  esac],[opt=false])
if test "x$opt" != xfalse; then
      CFLAGS="-O3 ${CFLAGS}"
      CXXFLAGS="-O3 ${CXXFLAGS}"
fi

# --enable-profile
AC_ARG_ENABLE([profile],
  [  --enable-profile    turn on compiler profiling],
  [case "${enableval}" in
    yes) profile=true ;;
    no)  profile=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-profile]) ;;
  esac],[profile=false])
if test "x$profile" != xfalse; then
      CFLAGS="-pg -fno-omit-frame-pointer ${CFLAGS}"
      CXXFLAGS="-pg -fno-omit-frame-pointer ${CXXFLAGS}"
fi

AC_SUBST(CFLAGS)
AC_SUBST(CXXLAGS)

# yacc/bison stuff
YFLAGS="-dvy -Dparse.trace" 
LFLAGS="-i"
AC_SUBST(YFLAGS)
AC_SUBST(LFLAGS)
AC_DEFINE([YYDEBUG], [1], [define to turn on yacc debugging])

# --enable-logging
AC_ARG_ENABLE([logging],
  [  --enable-logging        turn on console logging],
  [case "${enableval}" in
    yes) logging=true ;;
    no)  logging=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-logging]) ;;
  esac],[logging=false])
if test "x$logging" != xfalse; then
      AC_DEFINE(LOGGING_ENABLED,1,[define to turn on console logging])
fi 

# Doxygen
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN($PACKAGE, librp.dox, docs)

# output Makefiles
AC_CONFIG_FILES([
        librp.pc
        Makefile
])
AC_OUTPUT
