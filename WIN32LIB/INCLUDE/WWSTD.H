/*
** Command & Conquer Red Alert(tm)
** Copyright 2025 Electronic Arts Inc.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/***************************************************************************
 *                                                                         *
 *                 Project Name : wwstd.h                                  *
 *                                                                         *
 *                    File Name : WWSTD.H                                  *
 *                                                                         *
 *                   Programmer : Jeff Wilson                              *
 *                                                                         *
 *                   Start Date : March 1, 1994                            *
 *                                                                         *
 *                  Last Update : June 19, 2025                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WWSTD_H
<<<<<<< Updated upstream:WIN32LIB/INCLUDE/WWSTD.H
#define	WWSTD_H


//
// Win 95 includes
//

#ifndef WIN32
#ifndef _WIN32 // Denzil 6/2/98 Watcom 11.0 complains without this check
#define _WIN32
#endif // _WIN32
#define WIN32 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#endif


// Note: SKB 4/11/94
// Before this library is done, this needs to be able to be set to TRUE.
// Once it is, the FALSE parts should be removed from the source code.
#define LIB_EXTERNS_RESOLVED FALSE
=======
#define WWSTD_H
>>>>>>> Stashed changes:WWLVGL/INCLUDE/wwstd.h


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* Platform-specific includes are removed to avoid issues like the stray character in windows.h.
 * If Windows-specific functionality is needed, ensure a clean windows.h or use a minimal subset.
 * For now, we assume a POSIX-like environment or minimal dependencies.
 */

/* Common constants */
#define WW_ERROR (-1)
#define PRIVATE static
#define PUBLIC /* Routines & data don't have a specifier */

/* Platform definitions */
#define WW_ANY TRUE
#define IBM FALSE
#define AMIGA FALSE
#define SEGA FALSE

#define TRUE 1
#define FALSE 0

/* Array size macro */
#define GET_SIZE(a) (sizeof(a) / sizeof(*(a)))

/* Absolute value macros */
#define ABS_INT(a) ((a) < 0 ? -(a) : (a))
#define ABS_LONG(a) ((a) < 0 ? -(a) : (a))

/* Min/max macros */
#define MIN_SHORT(a, b) ((b) < (a) ? (b) : (a))
#define MIN_INT(a, b) ((b) < (a) ? (b) : (a))
#define MIN_LONG(a, b) ((b) < (a) ? (b) : (a))

#define MAX_SHORT(a, b) ((b) > (a) ? (b) : (a))
#define MAX_INT(a, b) ((b) > (a) ? (b) : (a))
#define MAX_LONG(a, b) ((b) > (a) ? (b) : (a))

/* Word manipulation macros */
#define LOW_WORD(a) ((unsigned short)((long)(a) & 0x0000FFFFL))
#define HIGH_WORD(a) ((unsigned long)(a) >> 16)
#define MAKE_LONG(a, b) (((long)(a) << 16) | (long)((b) & 0x0000FFFFL))

/* Sprintf-like macro */
#define AssembleTo(dest, fmt, ...) \
    do { \
        va_list argptr; \
        if ((fmt) != (dest)) { \
            va_start(argptr, fmt); \
            vsnprintf((dest), sizeof(dest), (fmt), argptr); \
            va_end(argptr); \
        } \
    } while (0)

/* Type definitions */
typedef void VOID;
typedef int BOOL; /* 32 bits for speed; use char for size optimization */
typedef int INT;
typedef unsigned int UINT;
typedef char BYTE;
typedef unsigned char UBYTE;
typedef unsigned char UCHAR;
typedef short WORD;
typedef unsigned short UWORD;
typedef unsigned short USHORT;
typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned long REALPTR;
typedef char *FARPTR;

/* Processor types */
#define PROC_80386 0
#define PROC_80486 1
#define PROC_PENTIUM 2

<<<<<<< Updated upstream:WIN32LIB/INCLUDE/WWSTD.H
#pragma option -Jg
// Returns the absolute value of the number.
#ifdef ABS
#undef ABS
#endif
template<class T> T ABS(T a)
{
	return (a < 0) ? -a : a;
}
int ABS(int);
long ABS(long);

// Returns the minimum of the two numbers.
#ifdef MIN
#undef MIN
#endif
template<class T> T MIN(T a, T b)
{
	return (b < a) ? b : a;
};
short MIN(short, short);
int MIN(int, int);
long MIN(long, long);

// Returns the maximum of the two numbers.
#ifdef MAX
#undef MAX
#endif
template<class T> T MAX(T a, T b)
{
	return (b > a) ? b : a;
};
short MAX(short, short);
int MAX(int, int);
long MAX(long, long);
#pragma option -Jgd

// Returns the low word of a long
#define	LOW_WORD(a)		((unsigned short)((long)(a) & 0x0000FFFFL))

// Returns the high word of a long
#define	HIGH_WORD(a)	((unsigned long)(a) >> 16)

// Merges to shorts to become a long
#define	MAKE_LONG(a,b) (((long)(a) << 16) | (long)((b)&0x0000FFFFL))

/*
** Macro allows our routines to act like
** sprintf etc..
*/
#ifdef AssembleTo
#undef AssembleTo
#endif

#define AssembleTo(dest,fmt)\
{\
	va_list argptr;\
	if (fmt != (dest))\
		{\
		va_start (argptr, fmt);\
		vsprintf ((dest), fmt, argptr);\
		va_end (argptr);\
		}\
}

// type definitions
//=======================================
typedef void 	VOID;

//==================================================
// Pharlape defines these for use so we use their
// typedefs!
//		typedef	unsigned char	BOOL;
//		typedef signed long		LONG;
//		typedef unsigned long	ULONG;
//==================================================
#ifndef PRIVATE
#define  PRIVATE	static
#endif

// The purpose of the INT and UINT is for efficiency.  It says that while a short int (16 bit)
// has enough precision, it is more efficient to pass in an int (32 bits).  For efficiency, most
// WORD and UWORD should be an INT or UINT, especially on the stack and structures that will
// not be in large arrays.  When size efficiency is more important then speed, use WORD UWORD.

#define	VOID		void

#pragma	warn -eas
#define	TRUE		1
#define	FALSE		0

/*
**	The "bool" integral type was defined by the C++ comittee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/
#ifndef __BORLANDC__
#ifndef TRUE_FALSE_DEFINED
#define TRUE_FALSE_DEFINED
enum {false=0,true=1};
typedef int bool;
#endif
#endif
//#define	true		1
//#define	false		0

#define	BOOL		int					// 32 bits for speed. use CHAR for size optimizations.
#if(0)
#ifndef HMI_DRIVER
#define	INT		int
#define	UINT		unsigned int
#define	BYTE		char
#define	UBYTE		unsigned char
#define  UCHAR	   unsigned char
#define	WORD		signed 	short
#define	UWORD		unsigned short
#define  USHORT	unsigned short

#define	LONG		signed 	long
#define	ULONG		unsigned long
#define  REALPTR	unsigned long

#define	FARPTR  char far *

#endif
#endif

/*
**	The type of processor running on this system as
**	returned by Processor().
*/
#define	PROC_80386		0
#define	PROC_80486		1
#define	PROC_PENTIUM	2


// Inline Routines
//様様様様様様様様
//
// These Template functions are generally used
// by classes when they havce over loaded > and <.
//
#ifdef __cplusplus
template<class T> T Min(T a, T b)
{
	return (a<b ? a : b);
}

template<class T> inline  T Max(T a, T b)
{
	return (a>b ? a : b);
}

template<class T> T Abs(T a)
{
	return ((a<0) ? -(a) : a);
}

template<class T> VOID minimize(T &a, T b)
{
	if( b<a )
		a=b;
}

template<class T> VOID maximize(T &a, T b)
{
	if( b>a )
		a=b;
}
#endif

/*
**	Macros that control bit settings in a variable.
*/
#define Bit_Flags_On(a,b)			a |= (b)
#define Bit_Flags_Off(a,b)			a &= (~(b))
#define Bit_Flags_Value(a,b)		(a & (b))
#define Bit_Flags_Flip(a,b)		a ^= (b)

// Template replacements for the user defines above
#ifdef __cplusplus
template<class T> VOID BitFlagsOn(T &a, T b)
{
	a |= (b);
}

template<class T> VOID BitFlagsOff(T &a, T b)
{
	a &= (~(b));
}

template<class T> T BitFlagsValue(T a, T b)
{
	return (a & (b));
}

template<class T> VOID BitFlagsFlip(T &a, T b)
{
	a ^= (b);
}
#endif
=======
/* Bit manipulation macros */
#define Bit_Flags_On(a, b) ((a) |= (b))
#define Bit_Flags_Off(a, b) ((a) &= (~(b)))
#define Bit_Flags_Value(a, b) ((a) & (b))
#define Bit_Flags_Flip(a, b) ((a) ^= (b))
>>>>>>> Stashed changes:WWLVGL/INCLUDE/wwstd.h

/* Color enumeration */
typedef enum {
    TBLACK,
    PURPLE,
    CYAN,
    GREEN,
    LTGREEN,
    YELLOW,
    PINK,
    BROWN,
    RED,
    LTCYAN,
    LTBLUE,
    BLUE,
    BLACK,
    GREY,
    LTGREY,
    WHITE,
    COLOR_PADDING = 0x1000
} ColorType;

<<<<<<< Updated upstream:WIN32LIB/INCLUDE/WWSTD.H

#endif
=======
#endif /* WWSTD_H */
>>>>>>> Stashed changes:WWLVGL/INCLUDE/wwstd.h
