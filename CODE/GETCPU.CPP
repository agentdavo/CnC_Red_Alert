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

/* $Header: /CounterStrike/GETCPU.CPP 1     3/03/97 10:24a Joe_bostic $*/
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : GETCPU                                                       *
 *                                                                                             *
 *                    File Name : GETCPU.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 6/26/96                                                      *
 *                                                                                             *
 *                  Last Update : June 26th 1996 [ST]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *   Example of interface to assembly language code to find CPU type                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 *   Get_CPU_Type -- interface to ASM detection code                                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


//#define WIN32
//#include <windows.h>
#include <stdio.h>
#include	<string.h>


#define	bool	int

#include <ra/cpuid.h>



/***********************************************************************************************
 * Get_CPU_Type -- Find out what kind of CPU we are running on                                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    int   - reference to cpu type                                                     *
 *           bool  - reference to mmx availability flag                                        *
 *           char* - ptr to buffer to receive chip vendor info                                 *
 *           int   - length of above buffer                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/26/96 10:15AM ST : Created                                                             *
 *=============================================================================================*/
void Get_CPU_Type(int & cpu_type, bool & mmx, char * vendor_id, int vendor_id_length)
{
	/*
	** Call the asm CPU detection code
	*/
	mmx = Detect_MMX_Availability();

	/*
	** Return the promised results
	*/
	cpu_type = (int)CPUType;
	char * vendor_ptr = &VendorID;
	strncpy(vendor_id, vendor_ptr, vendor_id_length);
}










