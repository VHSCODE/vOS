#include "timer.h"
#include "types.h"
#include "pthread.h"

#include "machine.h"

#include "stdio.h"
#include "stdlib.h"
void timer_routine(struct timer *my_timer)
{
    u32 tick_acumm = 0;
    while (g_machine.is_running)
    {
        pthread_mutex_lock(&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);
        pthread_cond_wait(&g_machine.cpu_ptr[my_timer->cpu_index].clock_tick,&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);

        tick_acumm++;
        //printf("timer1 -> tickcount => %d\n",tick_acumm);
        if(tick_acumm >= my_timer->ticks_to_signal){
            //Tock
            //printf("Timer Tock\n");
            tick_acumm = 0;
        }
        pthread_mutex_unlock(&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);
    }

    free(my_timer); 
    
}