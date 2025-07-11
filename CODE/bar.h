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

/* $Header: /CounterStrike/BAR.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BAR.H                                                        *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 08/16/96                                                     *
 *                                                                                             *
 *                  Last Update : August 16, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef BAR_H
#define BAR_H

/*
**	The "bool" integral type was defined by the C++ committee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/

#include "fixed.h"

/*
**	This is a manager for a progress (or other) bargraph. Such a graph consists of a fill
**	and a background region. The fill percentage of the bargraph is controlled by an
**	update value. The bargraph can be optionally outlined.
*/
class ProgressBarClass
{
	public:
		ProgressBarClass(int x, int y, int width, int height, int forecolor, int backcolor, int bordercolor=0);

		bool Update(fixed value);
		void Redraw(void) const;

	private:

		void Outline(void) const;
		bool Is_Horizontal(void) const;
		bool Is_Outlined(void) const {return(BorderColor != 0);}

		/*
		**	This is the upper left coordinates of the bargraph.
		*/
		int X,Y;

		/*
		**	This is the dimensions of the bargraph.
		*/
		int Width, Height;

		/*
		**	These are the colors to use when drawing the progress bar.
		*/
		int BarColor;
		int BackColor;
		int BorderColor;

		/*
		**	This is the current value of the bargraph.
		*/
		fixed CurrentValue;

		/*
		**	This is the current value as of the last time the bargraph was rendered.
		*/
		fixed LastDisplayCurrent;

		/*
		**	If the bargraph has been drawn at least once, then this flag will
		**	be true.
		*/
		unsigned IsDrawn:1;
};

#endif
