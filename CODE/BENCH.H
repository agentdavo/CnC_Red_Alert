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
 *                    File Name : BENCH.H                                  *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                                                         *
 *                   Start Date : July 17, 1996                            *
 *                                                                         *
 *                  Last Update : June 19, 2025                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef BENCH_H
#define BENCH_H

#include "ftimer.h"

<<<<<<< Updated upstream:CODE/BENCH.H
/*
**	The "bool" integral type was defined by the C++ committee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/
#ifndef __BORLANDC__
#ifndef TRUE_FALSE_DEFINED
#define TRUE_FALSE_DEFINED
enum {false=0,true=1};
typedef int bool;
#endif
#endif

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
=======
/* Benchmark structure for tracking performance */
typedef struct Benchmark {
    BasicTimer *clock;     /* Timer for clocking events */
    ULONG average;         /* Total time of tracked events */
    ULONG counter;         /* Number of events tracked */
    ULONG total_count;     /* Absolute total number of events */
} Benchmark;
>>>>>>> Stashed changes:CODE/bench.h

/* Maximum number of events for running average */
#define BENCH_MAXIMUM_EVENT_COUNT 256

/* Function prototypes for benchmark */
Benchmark *Benchmark_Create(VOID);
VOID Benchmark_Destroy(Benchmark *bench);
VOID Benchmark_Begin(Benchmark *bench, BOOL reset);
VOID Benchmark_End(Benchmark *bench);
VOID Benchmark_Reset(Benchmark *bench);
ULONG Benchmark_Value(const Benchmark *bench);
ULONG Benchmark_Count(const Benchmark *bench);

#endif /* BENCH_H */