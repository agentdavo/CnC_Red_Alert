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

/* $Header: /CounterStrike/BENCH.H 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BENCH.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/17/96                                                     *
 *                                                                                             *
 *                  Last Update : July 17, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#ifndef BENCH_H
#define BENCH_H

#include	"mpu.h"
#include	"ftimer.h"

/*
**	The "bool" integral type was defined by the C++ committee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/
/*
**	This is a timer access object that will fetch the internal Pentium
**	clock value.
*/
class PentiumTimerClass
{
	public:
		unsigned long operator () (void) const {unsigned long h;unsigned long l = Get_CPU_Clock(h);return((l >> 4) | (h << 28));}
		operator unsigned long (void) const {unsigned long h;unsigned long l = Get_CPU_Clock(h);return((l >> 4) | (h << 28));}
};


/*
**	A performance tracking tool object. It is used to track elapsed time. Unlike a simple clock, this
**	class will keep a running average of the duration. Typical use of this would be to benchmark some
**	process that occurs multiple times. By benchmarking an average time, inconsistencies in a particular
**	run can be overcome.
*/
class Benchmark
{
	public:
		Benchmark(void);

		void Begin(bool reset=false);
		void End(void);

		void Reset(void);
		unsigned long Value(void) const;
		unsigned long Count(void) const {return(TotalCount);}

	private:
		/*
		**	The maximum number of events to keep running average of. If
		**	events exceed this number, then older events drop off the
		**	accumulated time. This number needs to be as small as
		**	is reasonable. The larger this number gets, the less magnitude
		**	that the benchmark timer can handle. Example; At a value of
		**	256, the magnitude of the timer can only be 24 bits.
		*/
		enum {MAXIMUM_EVENT_COUNT=256};

		/*
		**	This is the timer the is used to clock the events.
		*/
		BasicTimerClass<PentiumTimerClass> Clock;

		/*
		**	The total time off all events tracked so far.
		*/
		unsigned long Average;

		/*
		**	The total number of events tracked so far.
		*/
		unsigned long Counter;

		/*
		**	Absolute total number of events (possibly greater than the
		**	number of events tracked in the average).
		*/
		unsigned long TotalCount;
};


#endif
