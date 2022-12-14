#include "Clock.h"
#include "pthread.h"
#include "CPU.h"

void clock_routine(u32 cpu_index)
{
    while (g_machine.is_running)
    {
        
        pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].clock_mutex);
        pthread_cond_signal(&g_machine.cpu_ptr[cpu_index].clock_tick);
        //Tick
        pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].clock_mutex);
        
        pthread_cond_broadcast(&g_machine.cpu_ptr[cpu_index].global_timer_lock);
    }
    
}