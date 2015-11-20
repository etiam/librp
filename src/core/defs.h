#ifndef __DEFS_H__
#define __DEFS_H__

#include <math.h>

#define TRUE		1
#define FALSE		0
#undef  PI
#define PI		3.141592654
#define R2D		180.0/PI
#define D2R		PI/180.0
#undef	MINFLOAT
#define MINFLOAT	-1.0e18
#undef	MAXFLOAT
#define	MAXFLOAT	1.0e18
#undef  EPSILON
#define EPSILON		1.0e-7

typedef unsigned char   uchar;
typedef unsigned char	boolean;
typedef unsigned int    uint;
typedef unsigned long   ulong;
typedef unsigned short  ushort;
typedef char            *string;

//const long      MAXLONG = 10;

#endif
