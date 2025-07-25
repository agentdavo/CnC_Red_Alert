;
;	Command & Conquer Red Alert(tm)
;	Copyright 2025 Electronic Arts Inc.
;
;	This program is free software: you can redistribute it and/or modify
;	it under the terms of the GNU General Public License as published by
;	the Free Software Foundation, either version 3 of the License, or
;	(at your option) any later version.
;
;	This program is distributed in the hope that it will be useful,
;	but WITHOUT ANY WARRANTY; without even the implied warranty of
;	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;	GNU General Public License for more details.
;
;	You should have received a copy of the GNU General Public License
;	along with this program.  If not, see <http://www.gnu.org/licenses/>.
;

;***************************************************************************
;**     C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S       **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Fix a selector                           *
;*                                                                         *
;*                    File Name : FFIXSEL.ASM                              *
;*                                                                         *
;*                   Programmer : Jeff Wilson                              *
;*                                                                         *
;*                   Start Date : March 28, 1994                           *
;*                                                                         *
;*                  Last Update : March 28, 1994   []                      *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   FixSelector -- Fix the Priviledge level of a selector                 *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *



IDEAL
P386
MODEL USE32 FLAT


EXTRN		   exit			: near
GLOBAL             FixSelector          :NEAR

;============================================================================
CODESEG

;***************************************************************************
;* FIXSELECTOR -- Fix the Priviledge level of a selector                   *
;*                                                                         *
;*                                                                         *
;*                                                                         *
;* INPUT:  sel          the selector to fix-up                             *
;*                                                                         *
;* OUTPUT: UWORD        The fixed up selector                              *
;*                                                                         *
;* WARNINGS:                                                               *
;*                                                                         *
;* HISTORY:                                                                *
;*   03/28/1994  jaw  Created.                                             *
;*=========================================================================*

PROC FixSelector 	C near 

	USES ecx,edx
	      
	ARG	sel:WORD
		
;  Copy the Table Bit and IOPL from the Current CS

; Something is wrong the program should not be here unthe any circunstance	
;	movzx	ecx,[sel]
;	xor	eax,eax
;	mov	ax,cs
;	and	ax,7
;	or	ax,cx
push 0
call exit
	
	ret
;====================
ENDP FixSelector	



END





