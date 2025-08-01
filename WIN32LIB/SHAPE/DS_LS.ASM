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
;**   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
;***************************************************************************
;*                                                                         *
;*                 Project Name : Draw Shape Routines for library.         *
;*                                                                         *
;*                    File Name : DS_LS.ASM                                *
;*                                                                         *
;*                   Programmer : Scott K. Bowen                           *
;*                                                                         *
;*                   Start Date : August 24, 1993                          *
;*                                                                         *
;*                  Last Update : June 2, 1994   [BR]                      *
;*                                                                         *
;*-------------------------------------------------------------------------*
;* Functions:                                                              *
;*   Left_Skip -- Skips bytes in a data stream				   *
;* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
;********************* Model & Processor Directives ************************
IDEAL
P386
MODEL USE32 FLAT


;******************************** Includes *********************************
INCLUDE "shape.inc"


;********************************* Code ************************************
	CODESEG

;***************************************************************************
;* Left_Skip -- Skips bytes in a data stream				   *
;*                                                                         *
;* INPUT:								   *
;*		ECX = number of uncompressed bytes to skip		   *
;*		ESI = shape (source) buffer data address		   *
;*		EDI = viewport (destination) address			   *
;*		[WidthCount] = shape's width				   *
;*									   *
;* OUTPUT:								   *
;*		ECX - negative # pixels overrun, or 0			   *
;*		EDX - XTotal initializer value (0 since there's no scaling)*
;*		ESI - updated to the current location in the shape data	   *
;*		EDI - incremented by # pixels overrun			   *
;*		[WidthCount] - decremented by # bytes skipped		   *
;*                                                                         *
;* WARNINGS:	none                                                       *
;*                                                                         *
;* HISTORY:                                                                *
;*   04/14/1992 PWG : Created.                                             *
;*   08/19/1993 SKB : Split drawshp.asm into several modules.              *
;*   06/02/1994 BR : Converted to 32-bit                                   *
;*=========================================================================*
PROC	Left_Skip NOLANGUAGE NEAR

	sub 	[WidthCount],ecx	; we process ECX bytes of real width

	;--------------------------------------------------------------------
	; Put shape data address in EDI so we can do a scasb on it
	;--------------------------------------------------------------------
	xchg	esi,edi			; xchange ESI and EDI
	jecxz	??out			; exit if ECX is 0 (no bytes to skip)

	;--------------------------------------------------------------------
	; Search through the string and count down the info we have handled.
	; If we find a run (0 followed by a count byte), then handle it.
	;--------------------------------------------------------------------
??cliptop:
	mov	eax,0			; set al to 0 (we're scanning for 0)
	repne	scasb			; scan through source data
	jz	??on_run		; if it is a run then deal with it
	jecxz	??out			; if we're done then get outta here

	;--------------------------------------------------------------------
	; If we have a run then get the next byte which is the length.  
	;--------------------------------------------------------------------
??on_run:
	mov	al,[BYTE PTR edi]	; get the count of zeros to run
	inc	edi			; advance past the count
	inc	ecx			; the 0 found doesn't count
	sub	ecx,eax			; subtract the count from remaining
	jg	??cliptop		; if more bytes left, scan again

	;--------------------------------------------------------------------
	; Put shape address back into ESI, adjust EDI
	;--------------------------------------------------------------------
??out:
	xchg	esi,edi			; xchange ESI and EDI
	sub	edi,ecx			; increment EDI by overrun pixels
	add	[WidthCount],ecx	; adjust by # bytes overrun
	mov	edx,0			; no scaling, so clear EDX
	ret				; return back to the real function

	ENDP	Left_Skip

	END

;**************************** End of ds_ls.asm *****************************
