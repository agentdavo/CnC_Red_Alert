#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "wwstd.h"

typedef struct TimerClass {
    long Started;
    long Accumulated;
} TimerClass;

static inline long TimerClass_Set(TimerClass *timer, long value, BOOL start)
{
    (void)timer; (void)value; (void)start; return 0;
}
static inline long TimerClass_Stop(TimerClass *timer) { (void)timer; return 0; }
static inline long TimerClass_Start(TimerClass *timer) { (void)timer; return 0; }
static inline long TimerClass_Reset(TimerClass *timer, BOOL start)
{
    return TimerClass_Set(timer, 0, start);
}
static inline long TimerClass_Time(TimerClass *timer) { (void)timer; return 0; }

typedef TimerClass CountDownTimerClass;

extern TimerClass TickCount;
extern CountDownTimerClass CountDown;

#endif /* TIMER_H */
