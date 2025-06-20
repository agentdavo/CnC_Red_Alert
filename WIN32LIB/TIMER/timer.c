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
 *                    File Name : TIMER.C                                  *
 *                                                                         *
 *                   Programmer : Scott K. Bowen                           *
 *                                                                         *
 *                   Start Date : July 6, 1994                             *
 *                                                                         *
 *                  Last Update : June 19, 2025                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <ftimer.h>
#include <stdlib.h>

/* Global timer system state */
BOOL TimerSystemOn = TRUE;

/* Timer tick source */
ULONG Timer(BaseTimerEnum type)
{
    (void)type; /* Ignore type for now; use single clock source */
    return (ULONG)(clock() / (CLOCKS_PER_SEC / 1000)); /* Milliseconds */
}

/* Basic timer functions */
BasicTimer *BasicTimer_Create(ULONG set)
{
    BasicTimer *timer = (BasicTimer *)malloc(sizeof(BasicTimer));
    if (timer) {
        timer->started = Timer(BT_SYSTEM) - set;
    }
    return timer;
}

VOID BasicTimer_Destroy(BasicTimer *timer)
{
    if (timer) free(timer);
}

ULONG BasicTimer_Value(const BasicTimer *timer)
{
    return timer ? Timer(BT_SYSTEM) - timer->started : 0;
}

/* Start/stop timer functions */
TTimer *TTimer_Create(BaseTimerEnum type, ULONG set, BOOL start)
{
    TTimer *timer = (TTimer *)malloc(sizeof(TTimer));
    if (timer) {
        timer->type = type;
        timer->accumulated = set;
        timer->started = start && TimerSystemOn ? Timer(type) + 1 : 0;
    }
    return timer;
}

VOID TTimer_Destroy(TTimer *timer)
{
    if (timer) free(timer);
}

ULONG TTimer_Value(const TTimer *timer)
{
    if (!timer) return 0;
    if (timer->started && TimerSystemOn) {
        ULONG ticks = Timer(timer->type);
        return timer->accumulated + (ticks - (timer->started - 1));
    }
    return timer->accumulated;
}

VOID TTimer_Stop(TTimer *timer)
{
    if (timer && timer->started && TimerSystemOn) {
        ULONG ticks = Timer(timer->type);
        timer->accumulated += ticks - (timer->started - 1);
        timer->started = 0;
    }
}

VOID TTimer_Start(TTimer *timer)
{
    if (timer && !timer->started && TimerSystemOn) {
        timer->started = Timer(timer->type) + 1;
    }
}

BOOL TTimer_Is_Active(const TTimer *timer)
{
    return timer && timer->started != 0;
}

LONG TTimer_Set(TTimer *timer, LONG value, BOOL start)
{
    if (!timer) return 0;
    timer->started = 0;
    timer->accumulated = value >= 0 ? (ULONG)value : 0;
    if (start && TimerSystemOn) {
        TTimer_Start(timer);
        return (LONG)TTimer_Value(timer);
    }
    return (LONG)timer->accumulated;
}

LONG TTimer_Reset(TTimer *timer, BOOL start)
{
    return TTimer_Set(timer, 0, start);
}

/* Countdown timer functions */
CDTimer *CDTimer_Create(BaseTimerEnum type, ULONG set, BOOL start)
{
    CDTimer *timer = (CDTimer *)malloc(sizeof(CDTimer));
    if (timer) {
        timer->type = type;
        timer->delay_time = set;
        timer->started = start && TimerSystemOn ? Timer(type) + 1 : 0;
    }
    return timer;
}

VOID CDTimer_Destroy(CDTimer *timer)
{
    if (timer) free(timer);
}

ULONG CDTimer_Value(const CDTimer *timer)
{
    if (!timer) return 0;
    ULONG remain = timer->delay_time;
    if (timer->started && TimerSystemOn) {
        ULONG value = Timer(timer->type) - (timer->started - 1);
        return value < remain ? remain - value : 0;
    }
    return remain;
}

VOID CDTimer_Stop(CDTimer *timer)
{
    if (timer && timer->started && TimerSystemOn) {
        timer->delay_time = CDTimer_Value(timer);
        timer->started = 0;
    }
}

VOID CDTimer_Start(CDTimer *timer)
{
    if (timer && !timer->started && TimerSystemOn) {
        timer->started = Timer(timer->type) + 1;
    }
}

BOOL CDTimer_Is_Active(const CDTimer *timer)
{
    return timer && timer->started != 0;
}

LONG CDTimer_Set(CDTimer *timer, LONG value, BOOL start)
{
    if (!timer) return 0;
    timer->started = 0;
    timer->delay_time = value >= 0 ? (ULONG)value : 0;
    if (start && TimerSystemOn) {
        CDTimer_Start(timer);
        return (LONG)CDTimer_Value(timer);
    }
    return (LONG)timer->delay_time;
}

LONG CDTimer_Reset(CDTimer *timer, BOOL start)
{
    return CDTimer_Set(timer, 0, start);
}