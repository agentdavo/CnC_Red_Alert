/*
**	Command & Conquer Red Alert(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/***************************************************************************
 **     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : wwstd.h                                  *
 *                                                                         *
 *                    File Name : WWLIB.H                                  *
 *                                                                         *
 *                   Programmer : Jeff Wilson                              *
 *                                                                         *
 *                   Start Date : March 1, 1994                            *
 *                                                                         *
 *                  Last Update : March 1, 1994   []                       *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WWSTD_H
#define	WWSTD_H

//
// Win 95 includes
//

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

// Note: SKB 4/11/94
// Before this library is done, this needs to be able to be set to TRUE.
// Once it is, the FALSE parts should be removed from the source code.
#define LIB_EXTERNS_RESOLVED FALSE

#include <stdio.h>
#include <stdlib.h>


//================================

// TRUE and FALSE are defined in pltypes.h

#ifndef IBM
#define IBM				TRUE
#endif

#ifndef AMIGA
#define AMIGA			FALSE
#endif

#ifndef	SEGA
#define	SEGA			FALSE
#endif

/*
**	Common constants used in normal code.
*/
#define WW_ERROR		-1

#ifdef NULL
#undef	NULL
#endif

#ifndef NULL
#define NULL		0L
#endif

#ifdef VOID
#undef VOID
#endif

#define PRIVATE		static
#define PUBLIC			/* Routines & data don't have a specifier */

#ifdef __cplusplus
#define	__CPPARGS	...
#else
#define	__CPPARGS
#endif

// This macro will get the size (in elements) of the specified array.
#ifdef GET_SIZE
#undef GET_SIZE
#endif
#define GET_SIZE(a)					((sizeof(a) / sizeof(*a)))

// Returns the absolute value of the number.
#ifdef ABS
#undef ABS
#endif
#define ABS(a) ((a) < 0 ? -(a) : (a))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

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

#define	TRUE		1
#define	FALSE		0

/*
**	The "bool" integral type was defined by the C++ comittee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/

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
#define Bit_Flags_On(a,b) ( (a) |= (b) )
#define Bit_Flags_Off(a,b) ( (a) &= ~(b) )
#define Bit_Flags_Value(a,b) ((a) & (b))
#define Bit_Flags_Flip(a,b) ( (a) ^= (b) )
#define	PROC_PENTIUM	2

//ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
//
// These Template functions are generally used
// by classes when they havce over loaded > and <.
//

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
	COLOR_PADDING=0x1000
} ColorType;
#endif
