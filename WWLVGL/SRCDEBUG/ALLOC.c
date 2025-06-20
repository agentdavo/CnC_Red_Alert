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
 *                 Project Name : Westwood Library                         *
 *                                                                         *
 *                    File Name : ALLOC.C                                  *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                                                         *
 *                   Start Date : February 1, 1992                         *
 *                                                                         *
 *                  Last Update : June 19, 2025                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Alloc -- Allocates system RAM.                                        *
 *   Ram_Free -- Determines the largest free chunk of RAM.                 *
 *   Free -- Free an Alloc'ed block of RAM.                                *
 *   Resize_Alloc -- Change the size of an allocated block.                *
 *   Heap_Size -- Size of the heap we have.                                *
 *   Total_Ram_Free -- Total amount of free RAM.                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <stdlib.h>
#include <string.h>
#include "wwmem.h"
#include "memflag.h"

/* Global variables */
ULONG MinRam = 0;    /* Record of least memory at worst case */
ULONG MaxRam = 0;    /* Record of total allocated at worst case */
static ULONG TotalRam = 0; /* Total RAM available */
static ULONG Memory_Calls = 0; /* Track allocation calls */
VOID (*Memory_Error)(VOID) = NULL;
VOID (*Memory_Error_Exit)(const char *string) = NULL;

/* DPMI stubs for legacy compatibility */
VOID DPMI_Lock(const VOID *ptr, LONG size)
{
    /* No-op in modern systems */
    (void)ptr;
    (void)size;
}

VOID DPMI_Unlock(const VOID *ptr, LONG size)
{
    /* No-op in modern systems */
    (void)ptr;
    (void)size;
}

/* Allocates system RAM with specified flags */
VOID *Alloc(ULONG bytes_to_alloc, MemoryFlagType flags)
{
    VOID *mem_ptr;
    ULONG original_size = bytes_to_alloc;

    /* Add space for flags */
    bytes_to_alloc += 1;

    /* Initialize total RAM if not set */
    if (!TotalRam) {
        TotalRam = Total_Ram_Free(MEM_NORMAL);
    }

    /* Allocate memory */
    mem_ptr = malloc(bytes_to_alloc);
    if (!mem_ptr) {
        if (Memory_Error) {
            Memory_Error();
        }
        return NULL;
    }

    /* Store flags */
    *(unsigned char *)mem_ptr = (unsigned char)flags;
    mem_ptr = (unsigned char *)mem_ptr + 1;

    /* Clear memory if requested */
    if (flags & MEM_CLEAR) {
        memset(mem_ptr, 0, original_size);
    }

    /* Update memory usage stats */
    LONG bytes_free = Total_Ram_Free(MEM_NORMAL);
    if (bytes_free >= 0 && (ULONG)bytes_free < MinRam) {
        MinRam = (ULONG)bytes_free;
    }
    if (TotalRam > (ULONG)bytes_free && TotalRam - (ULONG)bytes_free > MaxRam) {
        MaxRam = TotalRam - (ULONG)bytes_free;
    }

    Memory_Calls++;
    return mem_ptr;
}

/* Frees an allocated block of RAM */
VOID Free(const VOID *pointer)
{
    if (pointer) {
        VOID *base_ptr = (VOID *)((unsigned char *)pointer - 1);
        unsigned char flags = *(unsigned char *)base_ptr;

        /* Handle locked memory (legacy) */
        if (flags & MEM_LOCK) {
            /* No-op in modern systems */
        }

        free(base_ptr);
        Memory_Calls--;
    }
}

/* Resizes an allocated block */
VOID *Resize_Alloc(VOID *original_ptr, ULONG new_size_in_bytes)
{
    VOID *base_ptr = (VOID *)((unsigned char *)original_ptr - 1);
    unsigned char flags = *(unsigned char *)base_ptr; /* Save flags before realloc */
    VOID *new_ptr = realloc(base_ptr, new_size_in_bytes + 1);

    if (!new_ptr) {
        if (Memory_Error) {
            Memory_Error();
        }
        return NULL;
    }

    /* Restore flags */
    *(unsigned char *)new_ptr = flags;
    return (unsigned char *)new_ptr + 1;
}

/* Estimates largest free chunk of RAM */
LONG Ram_Free(MemoryFlagType flag)
{
    (void)flag;
    return 64 * 1024 * 1024; /* Assume 64MB free */
}

/* Estimates total heap size */
LONG Heap_Size(MemoryFlagType flag)
{
    (void)flag;
    if (!TotalRam) {
        TotalRam = Total_Ram_Free(MEM_NORMAL);
    }
    return TotalRam;
}

/* Estimates total free RAM */
LONG Total_Ram_Free(MemoryFlagType flag)
{
    (void)flag;
    return 64 * 1024 * 1024; /* Assume 64MB free */
}