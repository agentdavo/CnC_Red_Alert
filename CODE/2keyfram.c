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
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : 2KEYFRAM.C                              *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                                                         *
 *                   Start Date : June 25, 1995                            *
 *                                                                         *
 *                  Last Update : June 19, 2025                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Get_Build_Frame_Count -- Fetches the number of frames in data block.  *
 *   Get_Build_Frame_X -- Fetches the X offset of the shape image.         *
 *   Get_Build_Frame_Y -- Fetches the Y offset of the shape image.         *
 *   Get_Build_Frame_Width -- Fetches the width of the shape image.        *
 *   Get_Build_Frame_Height -- Fetches the height of the shape image.      *
 *   Get_Build_Frame_Palette -- Fetches the palette for the shape image.   *
 *   Build_Frame -- Builds a frame from keyframe data.                     *
 *   Reset_Theater_Shapes -- Resets theater-specific shape buffer.         *
 *   Reallocate_Big_Shape_Buffer -- Reallocates the big shape buffer.      *
 *   Disable_Uncompressed_Shapes -- Disables shape decompression.          *
 *   Enable_Uncompressed_Shapes -- Enables shape decompression.            *
 *   Check_Use_Compressed_Shapes -- Checks if uncompressed shapes are used.*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <string.h>
#include "function.h"
#include "memflag.h"

#define SUBFRAMEOFFS 7 /* 3 1/2 frame offsets loaded (2 offsets/frame) */
#define INITIAL_BIG_SHAPE_BUFFER_SIZE 8000000
#define THEATER_BIG_SHAPE_BUFFER_SIZE 4000000
#define UNCOMPRESS_MAGIC_NUMBER 56789
#define MAX_SLOTS 1500
#define THEATER_SLOT_START 1000
#define KF_KEYFRAME 0x01 /* Keyframe flag */
#define KF_DELTA 0x02    /* Delta frame flag */

#define Apply_Delta(buffer, delta) Apply_XOR_Delta((char *)(buffer), (char *)(delta))

/* Keyframe header structure */
typedef struct {
    USHORT frames;            /* Number of frames */
    USHORT x;                 /* X offset or slot index */
    USHORT y;                 /* Y offset or slot index */
    USHORT width;             /* Frame width */
    USHORT height;            /* Frame height */
    USHORT largest_frame_size; /* Largest frame size */
    SHORT flags;              /* Frame flags */
} KeyFrameHeaderType;

/* Shape header structure */
typedef struct {
    INT draw_flags;    /* Draw flags */
    char *shape_data;  /* Pointer to shape data */
    INT shape_buffer;  /* 1 if in theater buffer, 0 if in big buffer */
} ShapeHeaderType;

/* Global variables */
UINT BigShapeBufferLength = INITIAL_BIG_SHAPE_BUFFER_SIZE;
UINT TheaterShapeBufferLength = THEATER_BIG_SHAPE_BUFFER_SIZE;
char *BigShapeBufferStart = NULL;
char *TheaterShapeBufferStart = NULL;
BOOL UseBigShapeBuffer = FALSE;
BOOL OriginalUseBigShapeBuffer = FALSE;
BOOL IsTheaterShape = FALSE;
char *BigShapeBufferPtr = NULL;
INT TotalBigShapes = 0;
BOOL ReallocShapeBufferFlag = FALSE;
char *TheaterShapeBufferPtr = NULL;
INT TotalTheaterShapes = 0;
char *KeyFrameSlots[MAX_SLOTS];
INT TotalSlotsUsed = 0;
INT TheaterSlotsUsed = THEATER_SLOT_START;
static INT Length = 0;

/* Forward declarations for external functions */
extern VOID *LCW_Uncompress(const VOID *src, VOID *dst, INT dst_size);
extern VOID Apply_XOR_Delta(char *buffer, char *delta);
extern VOID Memory_Error_Handler(VOID);

/* Get shape header data */
VOID *Get_Shape_Header_Data(const VOID *ptr)
{
    if (UseBigShapeBuffer) {
        ShapeHeaderType *header = (ShapeHeaderType *)ptr;
        return (VOID *)(header->shape_data + (header->shape_buffer ? (long)TheaterShapeBufferStart : (long)BigShapeBufferStart));
    }
    return (VOID *)ptr;
}

/* Get last frame length */
INT Get_Last_Frame_Length(VOID)
{
    return Length;
}

/* Reset theater-specific shape buffer */
VOID Reset_Theater_Shapes(VOID)
{
    for (INT i = THEATER_SLOT_START; i < TheaterSlotsUsed; i++) {
        if (KeyFrameSlots[i]) {
            Free(KeyFrameSlots[i]);
            KeyFrameSlots[i] = NULL;
        }
    }
    TheaterShapeBufferPtr = TheaterShapeBufferStart;
    TotalTheaterShapes = 0;
    TheaterSlotsUsed = THEATER_SLOT_START;
}

/* Reallocate big shape buffer */
VOID Reallocate_Big_Shape_Buffer(VOID)
{
    if (ReallocShapeBufferFlag) {
        BigShapeBufferLength += 2000000; /* Extra 2 MB */
        BigShapeBufferPtr = BigShapeBufferPtr ? (char *)((long)BigShapeBufferPtr - (long)BigShapeBufferStart) : NULL;
        VOID *(*prev_error)(VOID) = Memory_Error;
        Memory_Error = NULL;
        BigShapeBufferStart = (char *)Resize_Alloc(BigShapeBufferStart, BigShapeBufferLength);
        Memory_Error = prev_error;
        if (!BigShapeBufferStart) {
            UseBigShapeBuffer = FALSE;
            BigShapeBufferPtr = NULL;
            return;
        }
        BigShapeBufferPtr = BigShapeBufferPtr ? (char *)((long)BigShapeBufferPtr + (long)BigShapeBufferStart) : BigShapeBufferStart;
        ReallocShapeBufferFlag = FALSE;
    }
}

/* Disable uncompressed shapes */
VOID Disable_Uncompressed_Shapes(VOID)
{
    UseBigShapeBuffer = FALSE;
}

/* Enable uncompressed shapes */
VOID Enable_Uncompressed_Shapes(VOID)
{
    UseBigShapeBuffer = OriginalUseBigShapeBuffer;
}

/* Check if uncompressed shapes should be used */
VOID Check_Use_Compressed_Shapes(VOID)
{
    /* Assume sufficient memory (>16MB) on modern systems */
    UseBigShapeBuffer = TRUE;
    OriginalUseBigShapeBuffer = TRUE;
}

/* Build a frame from keyframe data */
ULONG Build_Frame(const VOID *dataptr, USHORT framenumber, VOID *buffptr)
{
    char *ptr;
    ULONG offset[SUBFRAMEOFFS];
    KeyFrameHeaderType *keyfr;
    USHORT buffsize, currframe, subframe;
    INT length = 0;
    char frameflags;
    ULONG return_value;
    char *temp_shape_ptr;

    Length = 0;
    if (!dataptr || !buffptr) {
        return 0;
    }

    keyfr = (KeyFrameHeaderType *)dataptr;
    if (framenumber >= keyfr->frames) {
        return 0;
    }

    if (UseBigShapeBuffer) {
        if (!BigShapeBufferStart) {
            BigShapeBufferStart = (char *)Alloc(BigShapeBufferLength, MEM_NORMAL);
            BigShapeBufferPtr = BigShapeBufferStart;
            TheaterShapeBufferStart = (char *)Alloc(TheaterShapeBufferLength, MEM_NORMAL);
            TheaterShapeBufferPtr = TheaterShapeBufferStart;
        }

        if ((long)((long)BigShapeBufferStart + BigShapeBufferLength) - (long)BigShapeBufferPtr < 128000) {
            ReallocShapeBufferFlag = TRUE;
        }

        if (keyfr->x != UNCOMPRESS_MAGIC_NUMBER) {
            keyfr->x = UNCOMPRESS_MAGIC_NUMBER;
            if (IsTheaterShape) {
                keyfr->y = TheaterSlotsUsed;
                TheaterSlotsUsed++;
            } else {
                keyfr->y = TotalSlotsUsed;
                TotalSlotsUsed++;
            }
            KeyFrameSlots[keyfr->y] = (char *)Alloc(keyfr->frames * sizeof(char *), MEM_CLEAR);
        }

        if (KeyFrameSlots[keyfr->y][framenumber]) {
            return (ULONG)(IsTheaterShape ? TheaterShapeBufferStart : BigShapeBufferStart) + (ULONG)KeyFrameSlots[keyfr->y][framenumber];
        }
    }

    buffsize = keyfr->width * keyfr->height;
    ptr = (char *)((long)dataptr + ((framenumber << 3) + sizeof(KeyFrameHeaderType)));
    Mem_Copy(ptr, offset, 12L);
    frameflags = (char)(offset[0] >> 24);

    if (frameflags & KF_KEYFRAME) {
        ptr = (char *)((long)dataptr + (offset[0] & 0x00FFFFFFL));
        if (keyfr->flags & 1) {
            ptr = (char *)((long)ptr + 768L);
        }
        length = LCW_Uncompress(ptr, buffptr, buffsize);
    } else {
        if (frameflags & KF_DELTA) {
            currframe = (USHORT)offset[1];
            ptr = (char *)((long)dataptr + ((currframe << 3) + sizeof(KeyFrameHeaderType)));
            Mem_Copy(ptr, offset, SUBFRAMEOFFS * sizeof(ULONG));
        }

        ULONG offcurr = offset[1] & 0x00FFFFFFL;
        ULONG offdiff = (offset[0] & 0x00FFFFFFL) - offcurr;
        ptr = (char *)((long)dataptr + offcurr);
        if (keyfr->flags & 1) {
            ptr = (char *)((long)ptr + 768L);
        }
        length = LCW_Uncompress(ptr, buffptr, buffsize);
        if (length > buffsize) {
            return 0;
        }
        length = buffsize;
        Apply_Delta(buffptr, (char *)((long)ptr + offdiff));

        if (frameflags & KF_DELTA) {
            currframe++;
            subframe = 2;
            while (currframe <= framenumber) {
                offdiff = (offset[subframe] & 0x00FFFFFFL) - offcurr;
                length = buffsize;
                Apply_Delta(buffptr, (char *)((long)ptr + offdiff));
                currframe++;
                subframe += 2;
                if (subframe >= (SUBFRAMEOFFS - 1) && currframe <= framenumber) {
                    Mem_Copy((char *)((long)dataptr + ((currframe << 3) + sizeof(KeyFrameHeaderType))),
                             offset, SUBFRAMEOFFS * sizeof(ULONG));
                    subframe = 0;
                }
            }
        }
    }

    if (UseBigShapeBuffer) {
        if (IsTheaterShape) {
            return_value = (ULONG)TheaterShapeBufferPtr;
            temp_shape_ptr = TheaterShapeBufferPtr + keyfr->height + sizeof(ShapeHeaderType);
            if (3 & (long)temp_shape_ptr) {
                temp_shape_ptr = (char *)(((long)temp_shape_ptr + 3) & ~3);
            }
            memcpy(temp_shape_ptr, buffptr, length);
            ((ShapeHeaderType *)TheaterShapeBufferPtr)->draw_flags = -1;
            ((ShapeHeaderType *)TheaterShapeBufferPtr)->shape_data = temp_shape_ptr - (long)TheaterShapeBufferStart;
            ((ShapeHeaderType *)TheaterShapeBufferPtr)->shape_buffer = 1;
            KeyFrameSlots[keyfr->y][framenumber] = TheaterShapeBufferPtr - (long)TheaterShapeBufferStart;
            TheaterShapeBufferPtr = (char *)((long)temp_shape_ptr + length);
            if (3 & (long)TheaterShapeBufferPtr) {
                TheaterShapeBufferPtr = (char *)(((long)TheaterShapeBufferPtr + 3) & ~3);
            }
            Length = length;
            return return_value;
        } else {
            return_value = (ULONG)BigShapeBufferPtr;
            temp_shape_ptr = BigShapeBufferPtr + keyfr->height + sizeof(ShapeHeaderType);
            if (3 & (long)temp_shape_ptr) {
                temp_shape_ptr = (char *)(((long)temp_shape_ptr + 3) & ~3);
            }
            memcpy(temp_shape_ptr, buffptr, length);
            ((ShapeHeaderType *)BigShapeBufferPtr)->draw_flags = -1;
            ((ShapeHeaderType *)BigShapeBufferPtr)->shape_data = temp_shape_ptr - (long)BigShapeBufferStart;
            ((ShapeHeaderType *)BigShapeBufferPtr)->shape_buffer = 0;
            KeyFrameSlots[keyfr->y][framenumber] = BigShapeBufferPtr - (long)BigShapeBufferStart;
            BigShapeBufferPtr = (char *)((long)temp_shape_ptr + length);
            if (3 & (long)BigShapeBufferPtr) {
                BigShapeBufferPtr = (char *)(((long)BigShapeBufferPtr + 3) & ~3);
            }
            Length = length;
            return return_value;
        }
    }
    Length = length;
    return (ULONG)buffptr;
}

/* Fetch the number of frames in the data block */
USHORT Get_Build_Frame_Count(const VOID *dataptr)
{
    if (dataptr) {
        return ((KeyFrameHeaderType *)dataptr)->frames;
    }
    return 0;
}

/* Fetch the X offset of the shape image */
USHORT Get_Build_Frame_X(const VOID *dataptr)
{
    if (dataptr) {
        return ((KeyFrameHeaderType *)dataptr)->x;
    }
    return 0;
}

/* Fetch the Y offset of the shape image */
USHORT Get_Build_Frame_Y(const VOID *dataptr)
{
    if (dataptr) {
        return ((KeyFrameHeaderType *)dataptr)->y;
    }
    return 0;
}

/* Fetch the width of the shape image */
USHORT Get_Build_Frame_Width(const VOID *dataptr)
{
    if (dataptr) {
        return ((KeyFrameHeaderType *)dataptr)->width;
    }
    return 0;
}

/* Fetch the height of the shape image */
USHORT Get_Build_Frame_Height(const VOID *dataptr)
{
    if (dataptr) {
        return ((KeyFrameHeaderType *)dataptr)->height;
    }
    return 0;
}

/* Fetch the palette for the shape image */
BOOL Get_Build_Frame_Palette(const VOID *dataptr, VOID *palette)
{
    if (dataptr && (((KeyFrameHeaderType *)dataptr)->flags & 1)) {
        const char *ptr = (const char *)((long)dataptr +
                                         ((((long)sizeof(ULONG) << 1) *
                                           ((KeyFrameHeaderType *)dataptr)->frames) +
                                          16 + sizeof(KeyFrameHeaderType)));
        memcpy(palette, ptr, 768L);
        return TRUE;
    }
    return FALSE;
}