//
// Created by v on 27/10/22.
//

#include "scheduler.h"
#include "../core/CPU/CPU.h"
#include "stdio.h"
#include "stdlib.h"
#include "pcb.h"

#include "dispatcher.h"


struct pcb *get_next_pcb(u32 cpu_index);

void scheduler_routine(struct Timer *timer)
{
    while (g_machine.is_running)
    {

        pthread_mutex_lock(&timer->timer_mutex);
        pthread_cond_wait(&timer->timer_tick, &timer->timer_mutex);

        struct pcb *new_pcb = get_next_pcb(timer->cpu_index);

        if (new_pcb != NULL)
        {
            //Call to dispatcher. Find the next available hardware thread if possible.
            u32 res = dispatch(timer->cpu_index, new_pcb);

            if(res == DISPATCH_SUCCESS) //Only delete pcb if the process is assigned
                delete_last_pcb(new_pcb->priority, timer->cpu_index);
        }
        else
        {
            swap_queues(timer->cpu_index);
            printf("Swapping queues\n");
        }

        Step_Cpu(timer->cpu_index);

        pthread_mutex_unlock(&timer->timer_mutex);
    }

	free(timer);
}



struct pcb *get_next_pcb(u32 cpu_index)
{

    int i = 0;

    // Buscamos el primer queue que este lleno
    for (i; i < PRIORITY_COUNT; i++)
    {
        if (g_machine.cpu_ptr[cpu_index].run_queue.normal_queue->bitmap[i] == 1)
        {
            return get_last_pcb(i, cpu_index);
        }
    }

    return NULL;
}