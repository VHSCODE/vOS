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
        //Wait for clock tick
        pthread_mutex_lock(&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);
        pthread_cond_wait(&g_machine.cpu_ptr[my_timer->cpu_index].clock_tick,&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);

        tick_acumm++;

        if(tick_acumm >= my_timer->ticks_to_signal) //Time to emit timer tick
        {
            
            pthread_mutex_lock(&my_timer->timer_mutex);
            pthread_cond_signal(&my_timer->timer_tick); //Timer Tick
            pthread_mutex_unlock(&my_timer->timer_mutex);
            tick_acumm = 0;
        }
        pthread_mutex_unlock(&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);
    }

    free(my_timer);  //FIXME: Puede crear segmentation fault si el PGNR termina despues de esta linea.
    
}