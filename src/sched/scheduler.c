//
// Created by v on 27/10/22.
//

#include "scheduler.h"
#include "../core/timer.h"

#include "stdio.h"

#include "pcb.h"

struct pcb* get_next_pcb(u32 cpu_index);

void scheduler_routine(struct thread* thr){
		while (g_machine.is_running)
		{

			pthread_mutex_lock(&thr->timer->timer_mutex);
			pthread_cond_wait(&thr->timer->timer_tick, &thr->timer->timer_mutex);

			printf("Ejecutando scheduler\n");


			struct pcb* tmp = get_next_pcb(thr->timer->cpu_index);

			if(tmp != NULL)
			{
				printf("Executing process with PID=%d and Priority=%d\n", tmp->pid, tmp->priority);

				delete_last_pcb(tmp->priority, thr->timer->cpu_index);
			}
			else{
				swap_queues(thr->timer->cpu_index);
			}
			pthread_mutex_unlock(&thr->timer->timer_mutex);
		}
}

struct pcb* get_next_pcb(u32 cpu_index){

	int i =0;

	//Buscamos el primer queue que este lleno
	for(i; i < PRIORITY_COUNT; i++){
		if(g_machine.cpu_ptr[cpu_index].run_queue.normal_queue->bitmap[i] == 1){
			return get_last_pcb(i,cpu_index);
		}
	}

	return NULL;
}
