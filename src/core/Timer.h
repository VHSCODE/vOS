#ifndef VOS__TIMER_H_
#define VOS__TIMER_H_
#include "../types.h"

#include "pthread.h"

struct Timer{
    u32 cpu_index;
    u32 ticks_to_signal;
    
    pthread_cond_t timer_tick;
    pthread_mutex_t timer_mutex;
};
extern void timer_routine(struct Timer *my_timer);


#endif