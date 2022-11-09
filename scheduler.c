//
// Created by v on 27/10/22.
//

#include "scheduler.h"
#include "timer.h"

#include "stdio.h"

void scheduler_routine(struct thread* thr){
		while (g_machine.is_running)
		{

			pthread_mutex_lock(&thr->timer->timer_mutex);
			pthread_cond_wait(&thr->timer->timer_tick, &thr->timer->timer_mutex);

			printf("Ejecutando scheduler\n");

			pthread_mutex_unlock(&thr->timer->timer_mutex);
		}
}
