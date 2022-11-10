//
// Created by v on 30/09/22.
//

#include "stdlib.h"
#include <pthread.h>

#include "machine.h"
#include "timer.h"
#include "process_generator.h"
#include "clock.h"
#include "scheduler.h"
#include <string.h>



#define BASE_THREADS 5 //Un reloj de cpu, un temporizador y un generador de procesos, un scheduler y otro timer para el mismo.
#define MAX_THREADS 50
struct machine g_machine;

void init_machine(u32 cpu_count){

	g_machine.cpu_count = cpu_count;

	g_machine.cpu_ptr = calloc(cpu_count,sizeof(struct cpu));

	
	g_machine.is_running = 1;

	int i;
	//Allocate and start  threads per cpu
	for ( i = 0; i < g_machine.cpu_count; i++)
	{
		g_machine.cpu_ptr[i].threads = calloc(MAX_THREADS,sizeof(struct thread)); 
		g_machine.cpu_ptr[i].thread_count = BASE_THREADS;

		//Init queues

		g_machine.cpu_ptr[i].run_queue.normal_queue = malloc(sizeof (struct queue));
		memset(g_machine.cpu_ptr[i].run_queue.normal_queue->bitmap,0,PRIORITY_COUNT);

		g_machine.cpu_ptr[i].run_queue.expired_queue = malloc(sizeof (struct queue));
		memset(g_machine.cpu_ptr[i].run_queue.expired_queue->bitmap,0,PRIORITY_COUNT);

		pthread_cond_init(&g_machine.cpu_ptr[i].queue_cond, 0);
		pthread_mutex_init(&g_machine.cpu_ptr[i].queue_mutex, NULL);


		//Cpu clock
		{
			pthread_cond_init(&g_machine.cpu_ptr[i].clock_tick, 0);
			pthread_mutex_init(&g_machine.cpu_ptr[i].clock_mutex, NULL);

			strcpy(g_machine.cpu_ptr[i].threads[0].proc_name, "CPU-CLOCK");
			pthread_create(&g_machine.cpu_ptr[i].threads[0].thread_handle,
				NULL,
				(void*)clock_routine,
				i); //Clock
		}

		//Timer and process generator
		{
			struct timer* timer = malloc(sizeof(struct timer));

			timer->cpu_index = i;
			timer->ticks_to_signal = 10000;

			pthread_cond_init(&timer->timer_tick, 0);
			pthread_mutex_init(&timer->timer_mutex, 0);

			strcpy(g_machine.cpu_ptr[i].threads[1].proc_name, "PGNR-TIMER");
			pthread_create(&g_machine.cpu_ptr[i].threads[1].thread_handle, NULL, (void*)timer_routine, timer); //Timer



			g_machine.cpu_ptr[i].threads[2].timer = timer;

			strcpy(g_machine.cpu_ptr[i].threads[2].proc_name, "PGNR");	
			pthread_create(&g_machine.cpu_ptr[i].threads[2].thread_handle, NULL, (void *) process_generator_routine, &g_machine.cpu_ptr[i].threads[2]); //Process Generator
		}
		//Scheduler y su temporizador
		{

			struct timer *timer = malloc(sizeof (struct timer));

			timer->cpu_index =i;
			timer->ticks_to_signal = 100000/2;

			pthread_cond_init(&timer->timer_tick,0);
			pthread_mutex_init(&timer->timer_mutex,0);


			strcpy(g_machine.cpu_ptr[i].threads[3].proc_name, "SCHED-TIMER");
			pthread_create(&g_machine.cpu_ptr[i].threads[3].thread_handle,NULL,(void*)timer_routine,timer); //Timer del scheduler


			g_machine.cpu_ptr[i].threads[4].timer = timer;
			strcpy(g_machine.cpu_ptr[i].threads[4].proc_name, "SCHEDULER");
			pthread_create(&g_machine.cpu_ptr[i].threads[4].thread_handle, NULL, (void *) scheduler_routine, &g_machine.cpu_ptr[i].threads[4]); //Scheduler

		}
	}
	
	

}

//Esto se llamara con un boton o una combinacion de teclas, cualquier cosa mejor que un control c/d
void graceful_shutdown()
{
	g_machine.is_running = 0;
	int i,j;
	
	//Join every thread from each cpu
	for (i = 0; i < g_machine.cpu_count; i++)
	{
		for (j = 0; j < g_machine.cpu_ptr[i].thread_count; j++){
			pthread_join(g_machine.cpu_ptr[i].threads[j].thread_handle,NULL);
		}
	}
}


void deinit_machine(struct machine* machine)
{
	int i;
	
	for ( i = 0; i < g_machine.cpu_count; i++)
	{
		free(g_machine.cpu_ptr[i].threads);
	}

	free(g_machine.cpu_ptr);
}
