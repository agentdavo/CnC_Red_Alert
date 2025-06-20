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
 *                    File Name : MONO.H                                   *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                                                         *
 *                   Start Date : July 2, 1994                             *
 *                                                                         *
 *                  Last Update : June 19, 2025                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MONO_H
#define MONO_H

#include "wwstd.h"

/* Monochrome display context */
typedef struct MonoContext {
    char x;        /* Cursor X position */
    char y;        /* Cursor Y position */
    char attrib;   /* Default attribute */
    char page;     /* Current page */
} MonoContext;

/* Box drawing styles */
typedef enum {
    SINGLE,        /* Single thickness */
    DOUBLE_HORZ,   /* Double thick horizontal */
    DOUBLE_VERT,   /* Double thick vertical */
    DOUBLE,        /* Double thickness */
    BOX_STYLE_COUNT
} BoxStyleType;

/* Constants */
#define MONO_COLUMNS 80
#define MONO_LINES 25
#define MONO_DEFAULT_ATTRIBUTE 0x02
#define MONO_MAX_PAGES 16
#define MONO_SEGMENT 0xB000

/* Function prototypes for monochrome display */
MonoContext *Mono_Create(VOID);
VOID Mono_Destroy(MonoContext *mono);
VOID Mono_Enable(VOID);
VOID Mono_Disable(VOID);
BOOL Mono_Is_Enabled(VOID);
MonoContext *Mono_Get_Current(VOID);
VOID Mono_Draw_Box(MonoContext *mono, INT x, INT y, INT w, INT h, char attrib, BoxStyleType thick);
VOID Mono_Set_Default_Attribute(MonoContext *mono, char attrib);
VOID Mono_Clear(MonoContext *mono);
VOID Mono_Set_Cursor(MonoContext *mono, INT x, INT y);
VOID Mono_Print(MonoContext *mono, const char *text);
VOID Mono_Printf(MonoContext *mono, const char *fmt, ...);
VOID Mono_Text_Print(MonoContext *mono, const char *text, INT x, INT y, char attrib);
VOID Mono_View(MonoContext *mono);
INT Mono_Get_X(const MonoContext *mono);
INT Mono_Get_Y(const MonoContext *mono);

/* Global monochrome functions */
VOID Mono_Set_Cursor_Global(INT x, INT y);
INT Mono_Printf_Global(const char *fmt, ...);
VOID Mono_Clear_Screen(VOID);
VOID Mono_Text_Print_Global(const char *text, INT x, INT y, INT attrib);
VOID Mono_Draw_Rect(INT x, INT y, INT w, INT h, INT attrib, INT thick);
VOID Mono_Print_Global(const char *text);
INT Mono_X(VOID);
INT Mono_Y(VOID);

#endif /* MONO_H */