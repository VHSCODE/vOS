#include "Timer.h"
#include "../../Machine.h"
#include "CPU.h"

#include "stdlib.h"
#include "pthread.h"
void timer_routine(struct Timer *my_timer)
{
    u32 tick_acumm = 0;
    while (g_machine.is_running)
    {
        //Wait for clock tick

        pthread_mutex_lock(&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);
        pthread_cond_wait(&g_machine.cpu_ptr[my_timer->cpu_index].clock_tick,&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);

        tick_acumm++;

        if(tick_acumm >= my_timer->ticks_to_signal) //Time to emit Timer tick
        {
            
            pthread_mutex_lock(&my_timer->timer_mutex);
            pthread_cond_signal(&my_timer->timer_tick); //Timer Tick
			//printf("Timer tick\n");
            pthread_mutex_unlock(&my_timer->timer_mutex);
            tick_acumm = 0;
        }
        pthread_mutex_unlock(&g_machine.cpu_ptr[my_timer->cpu_index].clock_mutex);


        pthread_cond_wait(&g_machine.cpu_ptr[my_timer->cpu_index].global_timer_lock,&g_machine.cpu_ptr[my_timer->cpu_index].global_timer_mutex);
    }

    free(my_timer);
    
}