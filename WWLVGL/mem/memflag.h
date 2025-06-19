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
 * C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S
 ***************************************************************************
 *
 * Project Name : Memory System
 * File Name    : MEMFLAG.H
 * Programmer   : Jeff Wilson
 * Start Date   : April 4, 1994
 * Last Update  : June 19, 2025
 *
 ***************************************************************************/
#ifndef MEMFLAG_H
#define MEMFLAG_H

#include <stddef.h>

/* Memory allocation flags used by Alloc */
typedef enum {
    MEM_NORMAL = 0x0000,
    MEM_CLEAR  = 0x0002,
    MEM_REAL   = 0x0004,
    MEM_TEMP   = 0x0008,
    MEM_LOCK   = 0x0010
} MemoryFlagType;

/* Prototypes for VMPAGEIN.ASM */
void Force_VM_Page_In(void *buffer, int length);

/* Prototypes for ALLOC.C */
void *Alloc(unsigned long bytes_to_alloc, MemoryFlagType flags);
void Free(const void *pointer);
void DPMI_Lock(const void *ptr, long size);
void DPMI_Unlock(const void *ptr, long size);
void *Resize_Alloc(void *original_ptr, unsigned long new_size_in_bytes);
long Ram_Free(MemoryFlagType flag);
long Heap_Size(MemoryFlagType flag);
long Total_Ram_Free(MemoryFlagType flag);

/* Prototypes for MEM_COPY.ASM */
void Mem_Copy(const void *source, void *dest, unsigned long bytes_to_copy);

extern void (*Memory_Error)(void);
extern void (*Memory_Error_Exit)(char *string);
extern unsigned long MinRam;
extern unsigned long MaxRam;

#endif /* MEMFLAG_H */
