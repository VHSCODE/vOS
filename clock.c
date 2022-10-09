#include "clock.h"
#include "machine.h"

#include "timer.h"
#include "pthread.h"
#include "stdio.h"

void clock_routine(u32 cpu_index)
{


    while (g_machine.is_running)
    {
        
        pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].clock_mutex);
        pthread_cond_signal(&g_machine.cpu_ptr[cpu_index].clock_tick);
        //printf("Tick\n"); 
        pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].clock_mutex);
        
    }
    
}