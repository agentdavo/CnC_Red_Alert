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

#ifndef DIBAPI_H
#define DIBAPI_H

#include <stdint.h>
/*
 *  dibapi.h
 *
 *  Copyright (c) 1991 Microsoft Corporation. All rights reserved
 *
 *  Header file for Device-Independent Bitmap (DIB) API.  Provides
 *  function prototypes and constants for the following functions:
 *
 *  BitmapToDIB()        - Creates a DIB from a bitmap
 *  ChangeBitmapFormat() - Changes a bitmap to a specified DIB format
 *  ChangeDIBFormat()    - Changes a DIB's BPP and/or compression format
 *  CopyScreenToBitmap() - Copies entire screen to a standard Bitmap
 *  CopyScreenToDIB()    - Copies entire screen to a DIB
 *  CopyWindowToBitmap() - Copies a window to a standard Bitmap
 *  CopyWindowToDIB()    - Copies a window to a DIB
 *  CreateDIBPalette()   - Creates a palette from a DIB
 *  CreateDIB()          - Creates a new DIB
 *  DestroyDIB()         - Deletes DIB when finished using it
 *  DIBError()           - Displays message box with error message
 *  DIBHeight()          - Gets the DIB height
 *  DIBNumColors()       - Calculates number of colors in the DIB's color table
 *  DIBToBitmap()        - Creates a bitmap from a DIB
 *  DIBWidth()           - Gets the DIB width
 *  FindDIBBits()        - Sets pointer to the DIB bits
 *  GetSystemPalette()   - Gets the current palette
 *  LoadDIB()            - Loads a DIB from a file
 *  PaintBitmap()        - Displays standard bitmap in the specified DC
 *  PaintDIB()           - Displays DIB in the specified DC
 *  PalEntriesOnDevice() - Gets the number of palette entries
 *  PaletteSize()        - Calculates the buffer size required by a palette
 *  PrintDIB()           - Prints the specified DIB
 *  PrintScreen()        - Prints the entire screen
 *  PrintWindow()        - Prints all or part of a window
 *  SaveDIB()            - Saves the specified dib in a file
 *
 * See the file DIBAPI.TXT for more information about these functions.
 *
 *  ajw added
 *  LoadDIB_FromMemory() - Loads a DIB from BMP file data located at a location in memory.
 *
 */


/* Handle to a DIB */
#define HDIB HANDLE


/* Print Area selection */
#define PW_WINDOW        1
#define PW_CLIENT        2


/* Print Options selection */
#define PW_BESTFIT       1
#define PW_STRETCHTOPAGE 2
#define PW_SCALE         3

/* DIB Macros*/

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed 
// to hold those bits.

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

/* Error constants */
enum {
      ERR_MIN = 0,                     // All error #s >= this value
      ERR_NOT_DIB = 0,                 // Tried to load a file, NOT a DIB!
      ERR_MEMORY,                      // Not enough memory!
      ERR_READ,                        // Error reading file!
      ERR_LOCK,                        // Error on a GlobalLock()!
      ERR_OPEN,                        // Error opening a file!
      ERR_CREATEPAL,                   // Error creating palette.
      ERR_GETDC,                       // Couldn't get a DC.
      ERR_CREATEDDB,                   // Error create a DDB.
      ERR_STRETCHBLT,                  // StretchBlt() returned failure.
      ERR_STRETCHDIBITS,               // StretchDIBits() returned failure.
      ERR_SETDIBITSTODEVICE,           // SetDIBitsToDevice() failed.
      ERR_STARTDOC,                    // Error calling StartDoc().
      ERR_NOGDIMODULE,                 // Couldn't find GDI module in memory.
      ERR_SETABORTPROC,                // Error calling SetAbortProc().
      ERR_STARTPAGE,                   // Error calling StartPage().
      ERR_NEWFRAME,                    // Error calling NEWFRAME escape.
      ERR_ENDPAGE,                     // Error calling EndPage().
      ERR_ENDDOC,                      // Error calling EndDoc().
      ERR_SETDIBITS,                   // Error calling SetDIBits().
      ERR_FILENOTFOUND,                // Error opening file in GetDib()
      ERR_INVALIDHANDLE,               // Invalid Handle
      ERR_DIBFUNCTION,                 // Error on call to DIB function
      ERR_MAX                          // All error #s < this value
     };



/* Function prototypes */

HDIB      FAR  BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal);
HDIB      FAR  ChangeBitmapFormat (HBITMAP  hBitmap,
                                   uint16_t wBitCount,
                                   uint32_t dwCompression,
                                   HPALETTE hPal);
HDIB      FAR  ChangeDIBFormat (HDIB hDIB, uint16_t wBitCount,
                                uint32_t dwCompression);
HBITMAP   FAR  CopyScreenToBitmap (LPRECT);
HDIB      FAR  CopyScreenToDIB (LPRECT);
HBITMAP   FAR  CopyWindowToBitmap (HWND, uint16_t);
HDIB      FAR  CopyWindowToDIB (HWND, uint16_t);
HPALETTE  FAR  CreateDIBPalette (HDIB hDIB);
HDIB      FAR  CreateDIB(uint32_t, uint32_t, uint16_t);
uint16_t  FAR  DestroyDIB (HDIB);
void      FAR  DIBError (int ErrNo);
uint32_t  FAR  DIBHeight (LPCSTR lpDIB);
uint16_t  FAR  DIBNumColors (LPCSTR lpDIB);
HBITMAP   FAR  DIBToBitmap (HDIB hDIB, HPALETTE hPal);
uint32_t  FAR  DIBWidth (LPCSTR lpDIB);
LPSTR     FAR  FindDIBBits (LPCSTR lpDIB);
HPALETTE  FAR  GetSystemPalette (void);
HDIB      FAR  LoadDIB (LPSTR);
BOOL      FAR  PaintBitmap (HDC, LPRECT, HBITMAP, LPRECT, HPALETTE);
BOOL      FAR  PaintDIB (HDC, LPRECT, HDIB, LPRECT, HPALETTE);
int       FAR  PalEntriesOnDevice (HDC hDC);
uint16_t  FAR  PaletteSize (LPCSTR lpDIB);
uint16_t  FAR  PrintDIB (HDIB, uint16_t, uint16_t, uint16_t, LPSTR);
uint16_t  FAR  PrintScreen (LPRECT, uint16_t, uint16_t, uint16_t, LPSTR);
uint16_t  FAR  PrintWindow (HWND, uint16_t, uint16_t, uint16_t, uint16_t, LPSTR);
uint16_t  FAR  SaveDIB (HDIB, LPSTR);

//	ajw added
HDIB	LoadDIB_FromMemory( const unsigned char* pData, uint32_t dwBitsSize );

#endif
