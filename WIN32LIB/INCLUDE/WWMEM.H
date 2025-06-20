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
 *                    File Name : WWMEM.H                                  *
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

#ifndef WWMEM_H
#define WWMEM_H

#include "wwstd.h"
#include "memflag.h"

/* Function prototypes for memory management */
int Mem_Init(VOID *buffer, LONG size);
VOID *Mem_Alloc(VOID *poolptr, LONG lsize, ULONG id);
int Mem_Free(VOID *poolptr, VOID *buffer);
VOID Mem_Reference(VOID *node);
VOID Mem_Lock_Block(VOID *node);
VOID Mem_In_Use(VOID *node);
VOID *Mem_Find(VOID *poolptr, ULONG id);
ULONG Mem_Get_ID(VOID *node);
VOID *Mem_Find_Oldest(VOID *poolptr);
VOID *Mem_Free_Oldest(VOID *poolptr);
LONG Mem_Pool_Size(VOID *poolptr);
LONG Mem_Avail(VOID *poolptr);
LONG Mem_Largest_Avail(VOID *poolptr);
VOID Mem_Cleanup(VOID *poolptr);

#endif /* WWMEM_H */