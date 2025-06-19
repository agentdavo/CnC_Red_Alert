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
 *                 Project Name : Westwood Win32 Library                   *
 *                                                                         *
 *                    File Name : DDRAW.CPP                                *
 *                                                                         *
 *                   Programmer : Philip W. Gorrow                         *
 *                                                                         *
 *                   Start Date : October 10, 1995                         *
 *                                                                         *
 *                  Last Update : October 10, 1995   []                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*=========================================================================*/
/* The following PRIVATE functions are in this file:                       */
/*=========================================================================*/


/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/


#include "misc.h"
#include <gbuffer.h>
#include <palette.h>
#include "../INCLUDE/surface_api.h"
#include <stdio.h>
#include "../../src/debug_log.h"


LPDIRECTDRAW		DirectDrawObject=NULL;	// Pointer to the direct draw object
LPDIRECTDRAW2	DirectDraw2Interface = NULL;  	// Pointer to direct draw 2 interface
static struct WWSurface PrimarySurface;

HWND			MainWindow;					// Handle to programs main window
												// this is passed to SetCooperativeLevel
												// so DirectDraw knows which window is ours


PALETTEENTRY				PaletteEntries[256];		// 256 windows palette entries
LPDIRECTDRAWPALETTE	PalettePtr;					// Pointer to direct draw palette object
BOOL						FirstPaletteSet=FALSE;	// Is this the first time 'Set_Palette' has been called?
LPDIRECTDRAWSURFACE	PaletteSurface=NULL;
SurfaceMonitorClass	AllSurfaces;				//List of all direct draw surfaces
BOOL						CanVblankSync = TRUE;

BOOL						SystemToVideoBlits =FALSE;	// Does hardware support system mem to video mem blits?
BOOL						VideoToSystemBlits =FALSE;	// Does hardware support video mem to system mem blits?
BOOL						SystemToSystemBlits = FALSE; 	// Does hardware support system mem to system mem blits?
BOOL						OverlappedVideoBlits = TRUE;	// Can video driver blit overlapped regions?

/*
** Function to call if we detect focus loss
*/
extern	void (*Misc_Focus_Loss_Function)(void) = NULL;
extern	void (*Misc_Focus_Restore_Function)(void) = NULL;


/***********************************************************************************************
 * Process_DD_Result -- Does a message box based on the result of a DD command                 *
 *                                                                                             *
 * INPUT:		HRESULT result				- the result returned from the direct draw command		  *
 *             int     display_ok_msg	- should a message be displayed if command ok			  *                                                                                      *
 *                                                                                             *
 * OUTPUT:		none																									  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/27/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void Process_DD_Result(HRESULT result, int display_ok_msg)
{
    (void)display_ok_msg;
    char buf[64];
    snprintf(buf, sizeof(buf), "DD result: %d", (int)result);
    LOG_MSG(buf);
}



/***********************************************************************************************
 * Check_Overlapped_Blit_Capability -- See if video driver supports blitting overlapped regions*
 *                                                                                             *
 *  We will check for this by drawing something to a video page and blitting it over itself.   *
 * If we end up with the top line repeating then overlapped region blits dont work.            *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/7/96 5:06PM ST : Created                                                               *
 *=============================================================================================*/
void Check_Overlapped_Blit_Capability(void)
{

	/*
	** Assume we can until we find out otherwise
	*/
	OverlappedVideoBlits = TRUE;

	GraphicBufferClass test_buffer;

	test_buffer.Init (64, 64, NULL, 0, (GBC_Enum)GBC_VIDEOMEM);

	test_buffer.Clear();

	/*
	** Plot a pixel in the top left corner of the buffer.
	*/
	test_buffer.Put_Pixel(0, 0, 255);

	/*
	** Blit the buffer down by one line. If we end up with a vertical strip of pixel 255's then
	** overlapped blits dont work
	*/

	test_buffer.Blit(test_buffer, 0, 0, 0, 1, test_buffer.Get_Width(), test_buffer.Get_Height()-1);

	if (test_buffer.Get_Pixel (0 ,5) == 255) OverlappedVideoBlits = FALSE;
}



/***********************************************************************************************
 * Set_Video_Mode -- Initializes Direct Draw and sets the required Video Mode                  *
 *                                                                                             *
 * INPUT:  		int width   			- the width of the video mode in pixels						  *
 *					int height           - the height of the video mode in pixels                   *
 *					int bits_per_pixel	- the number of bits per pixel the video mode supports     *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/26/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
BOOL Set_Video_Mode(HWND hwnd, int w, int h, int bits_per_pixel)
{
    (void)hwnd;
    (void)bits_per_pixel;
    surface_init(&PrimarySurface, w, h);
    return TRUE;

}

/***********************************************************************************************
 * Reset_Video_Mode -- Resets video mode and deletes Direct Draw Object                        *
 *                                                                                             *
 * INPUT:		none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/26/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void Reset_Video_Mode(void)
{
        if(PrimarySurface.canvas) {
                lv_obj_del(PrimarySurface.canvas);
                lv_draw_buf_destroy(PrimarySurface.draw_buf);
                PrimarySurface.canvas = NULL;
                PrimarySurface.draw_buf = NULL;
        }
}





/***********************************************************************************************
 * Get_Free_Video_Memory -- returns amount of free video memory                                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   bytes of available video RAM                                                      *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/29/95 12:52PM ST : Created                                                            *
 *=============================================================================================*/
unsigned int Get_Free_Video_Memory(void)
{
        return 0;
}

 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   hardware flags                                                                    *
 *                                                                                             *
 * WARNINGS: Must call Set_Video_Mode 1st to create the direct draw object                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    1/12/96 9:14AM ST : Created                                                              *
 *=============================================================================================*/
unsigned Get_Video_Hardware_Capabilities(void)
{
        return 0;
}

 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *=============================================================================================*/
extern int ScreenWidth;
void Wait_Vert_Blank(void)
{
        (void)ScreenWidth;
        lv_timer_handler();
}





/***********************************************************************************************
 * Set_Palette -- set a direct draw palette                                                    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to 768 rgb palette bytes                                                      *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/11/95 3:33PM ST : Created                                                             *
 *=============================================================================================*/
void Set_DD_Palette ( void *palette )
{
        if (!palette) return;
        surface_set_palette(&PrimarySurface, (const unsigned char*)palette);
}






/***********************************************************************************************
 * Wait_Blit -- waits for the DirectDraw blitter to become idle                                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07-25-95 03:53pm ST : Created                                                             *
 *=============================================================================================*/

void Wait_Blit (void)
{
        lv_timer_handler();
}



/***********************************************************************************************
 * SMC::SurfaceMonitorClass -- constructor for surface monitor class                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:23PM ST : Created                                                              *
 *=============================================================================================*/

SurfaceMonitorClass::SurfaceMonitorClass(void)
{
	for (int i=0 ; i<MAX_SURFACES ; i++)
	{
		Surface[i]=NULL;
	}
	InFocus=FALSE;
	SurfacesRestored=FALSE;
}


/***********************************************************************************************
 * SMC::Add_DD_Surface -- add a new surface to the list                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to surface                                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *                 11/3/95 3:24PM ST : Created                                                 *
 *=============================================================================================*/
void SurfaceMonitorClass::Add_DD_Surface (LPDIRECTDRAWSURFACE new_surface)
{
	if ( !Got_Surface_Already (new_surface) ){
		for (int i=0 ; i<MAX_SURFACES ; i++)
		{
			if ( Surface[i]==NULL ){
				Surface[i]=new_surface;
				return;
			}
		}
	}
}


/***********************************************************************************************
 * SMC::Remove_DD_Surface -- remove a direct draw surface from the list                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to Surface                                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:25PM ST : Created                                                              *
 *=============================================================================================*/

void SurfaceMonitorClass::Remove_DD_Surface (LPDIRECTDRAWSURFACE old_surface)
{
	for (int i=0 ; i<MAX_SURFACES ; i++)
	{
		if ( Surface[i]==old_surface ){
			Surface[i]=NULL;
			return;
		}
	}
}


/***********************************************************************************************
 * SMC::Got_Surface_Already -- check if a surface is already in the list                       *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to surface                                                                    *
 *                                                                                             *
 * OUTPUT:   True if surface is in list                                                        *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:25PM ST : Created                                                              *
 *=============================================================================================*/

BOOL SurfaceMonitorClass::Got_Surface_Already (LPDIRECTDRAWSURFACE test_surface)
{
	for (int i=0 ; i<MAX_SURFACES ; i++)
	{
		if ( Surface[i]==test_surface ){
			return(TRUE);
		}
	}
	return (FALSE);
}

/***********************************************************************************************
 * SMC::Restore_Surfaces -- restore the direct draw surfaces in the list                       *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:26PM ST : Created                                                              *
 *=============================================================================================*/

void SurfaceMonitorClass::Restore_Surfaces (void)
{
	if (InFocus){
		/*
		** Call restore for each Direct Draw surface
		*/
		for (int i=0 ; i<MAX_SURFACES ; i++)
		{
			if ( Surface[i] ){
				if (Surface[i]->Restore() != DD_OK){
					if (Misc_Focus_Loss_Function){
						Misc_Focus_Loss_Function();
					}
					return;
				}
			}
		}

		/*
		** PWG/ST: Now that we know all the surfaces are restored call
		** the function pointer to notify the program that it has
		** happened.  This function pointer is used to clear the pages,
		** etc.
		*/
		if (Misc_Focus_Restore_Function){
			Misc_Focus_Restore_Function();
		}

		SurfacesRestored = TRUE;

		/*
		** Restore the palette
		*/
		Set_DD_Palette (CurrentPalette);
	}
}


/***********************************************************************************************
 * SMC::Set_Surface_Focus -- set the InFocus flag to the given state                           *
 *                                                                                             *
 *  The InFocus flag is used to keep track of whether our application is currently in focus.   *
 *  We dont want to be restoring video surfaces when we are supposed to be running in the      *
 *  background.                                                                                *
 *                                                                                             *
 * INPUT:    bool in focus                                                                     *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/6/95 12:21PM ST : Created                                                             *
 *=============================================================================================*/

void SurfaceMonitorClass::Set_Surface_Focus ( BOOL in_focus )
{
	InFocus=in_focus;
}




/***********************************************************************************************
 * SMC::Release -- releases all direct draw surfaces                                           *
 *                                                                                             *
 *  Call this at the end of the game before called RestoreDisplayMode                          *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/6/96 12:23PM ST : Created                                                              *
 *=============================================================================================*/

void SurfaceMonitorClass::Release(void)
{
	/*
	** Call release for each Direct Draw surface
	*/
	for (int i=0 ; i<MAX_SURFACES ; i++)
	{
		if ( Surface[i] ){
			Surface[i]->Release();
			Surface[i] = 0;
		}
	}

}

