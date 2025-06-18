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
 **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : WSA 32bit LIbrary                        *
 *                                                                         *
 *                    File Name : WSA.H                                    *
 *                                                                         *
 *                   Programmer : Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : May 23, 1994                             *
 *                                                                         *
 *                  Last Update : May 25, 1994   [SKB]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Open_Animation -- file name and flags, system allocates buffer.       *
 *   Open_Animation -- file name, flags, palette, system allocates buffer. *
 *   Open_Animation -- file_name, graphic buffer, flags.                   *
 *   Open_Animation -- file name, bufferclass, flags, palette.             *
 *   Open_Animation -- filename, ptr, size, flags, no palette.             *
 *   Animate_Frame -- Animate a frame to a page with magic colors.         *
 *   Animate_Frame -- Animate a frame to a viewport with magic colors.     *
 *   Animate_Frame -- Animate a frame to a page.                           *
 *   Animate_Frame -- Animate a frame to a viewport.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef WSA_H
#define WSA_H

#ifndef WWSTD_H
#include "wwstd.h"
#endif

#ifndef GBUFFER_H
#include "gbuffer.h"
#endif

#ifndef VBUFFER_H
#include "vbuffer.h"
#endif

//lint -strong(AJX,WSAType)
typedef enum {
	WSA_NORMAL,								// Normal WSA animation
	WSA_GHOST	 	= 0x1000,			// Or'd with the above flags to get ghosting
	WSA_PRIORITY2 	= 0x2000,			// Copy using a priority (or in the priority)
	WSA_TRANS    	= 0x4000,			// Copy frame, ignoring transparent colors
	WSA_PRIORITY 	= 0x8000				// Copy using a priority (or in the priority)
} WSAType;


//lint -strong(AJX,WSAOpenType)
typedef enum {
	WSA_OPEN_FROM_MEM		= 0x0000,	// Try to load entire anim into memory.
	WSA_OPEN_INDIRECT		= 0x0000,	// First animate to internal buffer, then copy to page/viewport.
	WSA_OPEN_FROM_DISK	= 0x0001,	// Force the animation to be disk based.	
	WSA_OPEN_DIRECT		= 0x0002,	// Animate directly to page or viewport.

	// These next two have been added for the 32 bit library to give a better idea of what is
	// happening.  You may want to animate directly to the destination or indirectly to the
	// destination by using the animations buffer.  Indirecly is best if the dest is a seenpage
	// and the animation is not linear or if the destination is modified between frames.
	WSA_OPEN_TO_PAGE  = WSA_OPEN_DIRECT ,
	WSA_OPEN_TO_BUFFER= WSA_OPEN_INDIRECT ,

} WSAOpenType;

/*=========================================================================*/
/* The following prototypes are for the file: WSA.CPP								*/
/*=========================================================================*/

VOID * cdecl Open_Animation(BYTE const *file_name, BYTE *user_buffer, LONG user_buffer_size, WSAOpenType user_flags, UBYTE *palette=NULL);
VOID cdecl Close_Animation( VOID *handle );
BOOL cdecl Animate_Frame(VOID *handle, GraphicViewPortClass& view,
                         WORD frame_number, WORD x_pixel=0, WORD y_pixel=0,
                         WSAType flags_and_prio = WSA_NORMAL, VOID *magic_cols=NULL, VOID *magic=NULL);
WORD cdecl Get_Animation_Frame_Count(VOID *handle);
BOOL cdecl Animate_Frame(VOID *handle, VideoViewPortClass& view,
                         WORD frame_number, WORD x_pixel=0, WORD y_pixel=0,
                         WSAType flags_and_prio = WSA_NORMAL, VOID *magic_cols=NULL, VOID *magic=NULL);
WORD cdecl Get_Animation_Frame_Count(VOID *handle);
WORD cdecl Get_Animation_X(VOID const *handle);
WORD cdecl Get_Animation_Y(VOID const *handle);
WORD cdecl Get_Animation_Width(VOID const *handle);
WORD cdecl Get_Animation_Height(VOID const *handle);
WORD cdecl Get_Animation_Palette(VOID const *handle);
ULONG cdecl Get_Animation_Size(VOID const *handle);


/***************************************************************************
 * OPEN_ANIMATION -- file name, flags, palette, system allocates buffer.   *
 *                                                                         *
 *                                                                         *
 * INPUT:   BYTE *file_name - name of file to open.                        *
 *          WSAOpenType user_flags - flags on how to open.                 *
 *          UBYTE *palette - pointer to a palette buffer to fill.          *
 *                                                                         *
 * OUTPUT:  VOID *pointer to animation data.  Must be used for all 			*
 *               other WSA calls.                                          *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/24/1994 SKB : Created.                                             *
 *=========================================================================*/
inline VOID *cdecl Open_Animation(BYTE *file_name, WSAOpenType user_flags, UBYTE *palette=NULL) 
{
	return (Open_Animation(file_name, NULL, 0L, user_flags,  palette));
}


/***************************************************************************
 * OPEN_ANIMATION -- file_name, bufferclass, flags. 								*
 *                                                                         *
 *                                                                         *
 * INPUT:   BYTE *file_name - name of file to open.                        *
 *          GraphicBufferClass - pointer to a buffer.                      *
 *          WSAOpenType user_flags - flags on how to open.                 *
 *          UBYTE *palette - pointer to a palette buffer to fill.          *
 *                                                                         *
 * OUTPUT:  VOID *pointer to animation data.  Must be used for all 			*
 *               other WSA calls.                                          *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/24/1994 SKB : Created.                                             *
 *=========================================================================*/
inline VOID *cdecl Open_Animation(BYTE *file_name, BufferClass& buffer, WSAOpenType user_flags,  UBYTE *palette=NULL) 
{
	return (Open_Animation(file_name, (BYTE *)buffer.Get_Buffer(), buffer.Get_Size(), user_flags, palette));
}


/*=========================================================================*/
/* The following prototypes are for the file: LP_ASM.ASM							*/
/*=========================================================================*/


extern "C" {
UWORD Apply_XOR_Delta(BYTE *source_ptr, BYTE *delta_ptr);
VOID  Apply_XOR_Delta_To_Page_Or_Viewport(VOID *target, VOID *delta, WORD width, WORD nextrow, WORD copy);
}



#endif // WSA_H
