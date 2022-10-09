#ifndef VOS__TIMER_H_
#define VOS__TIMER_H_
#include "types.h"

struct timer{
    u32 cpu_index;
    u32 timer_thread_index;
    u32 ticks_to_signal;
};
extern void timer_routine(struct timer *my_timer);


#endif