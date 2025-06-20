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
 *                 Project Name : Memory System                            *
 *                                                                         *
 *                    File Name : MEMFLAG.H                                *
 *                                                                         *
 *                   Programmer : Jeff Wilson                              *
 *                                                                         *
 *                   Start Date : April 4, 1994                            *
 *                                                                         *
 *                  Last Update : June 19, 2025                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MEMFLAG_H
#define MEMFLAG_H

#include "wwstd.h"

/* Memory allocation flags for Alloc */
typedef enum {
    MEM_NORMAL = 0x0000, /* Default memory (normal) */
    MEM_NEW = 0x0001,    /* Called by allocation routines */
    MEM_CLEAR = 0x0002,  /* Clear memory before returning */
    MEM_REAL = 0x0004,   /* Real-mode memory (legacy) */
    MEM_TEMP = 0x0008,   /* Temporary memory */
    MEM_LOCK = 0x0010    /* Lock memory (legacy) */
} MemoryFlagType;

/* Function prototypes for memory management */
VOID *Alloc(ULONG bytes_to_alloc, MemoryFlagType flags);
VOID Free(const VOID *pointer);
VOID DPMI_Lock(const VOID *ptr, LONG size);
VOID DPMI_Unlock(const VOID *ptr, LONG size);
VOID *Resize_Alloc(VOID *original_ptr, ULONG new_size_in_bytes);
LONG Ram_Free(MemoryFlagType flag);
LONG Heap_Size(MemoryFlagType flag);
LONG Total_Ram_Free(MemoryFlagType flag);
VOID Force_VM_Page_In(VOID *buffer, INT length);
VOID Mem_Copy(const VOID *source, VOID *dest, ULONG bytes_to_copy);

/* Memory error handling */
extern VOID (*Memory_Error)(VOID);
extern VOID (*Memory_Error_Exit)(const char *string);

/* Memory usage tracking */
extern ULONG MinRam; /* Least memory at worst case */
extern ULONG MaxRam; /* Total allocated at worst case */

#endif /* MEMFLAG_H */