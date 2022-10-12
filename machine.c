//
// Created by v on 30/09/22.
//

#include "stdlib.h"
#include <pthread.h>

#include "machine.h"
#include "timer.h"
#include "process_generator.h"
#include "clock.h"

#include <string.h>

#define BASE_THREADS 3 //One cpu clock, one timer and one process_generator
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

		//Init clock mutex and condition variables
		pthread_cond_init(&g_machine.cpu_ptr[i].clock_tick,0);
		pthread_mutex_init(&g_machine.cpu_ptr[i].clock_mutex,NULL);

		//Init threads for every routine
		
		strcpy(g_machine.cpu_ptr[i].threads[0].proc_name, "CPU-CLOCK");	
		pthread_create(&g_machine.cpu_ptr[i].threads[0].thread_handle, NULL, (void*)clock_routine, i); //Clock
				
		//Timer and process generator
		{
			//https://stackoverflow.com/questions/39300744/how-to-pass-a-struct-by-value-to-a-pthread
			struct timer timer;
			timer.cpu_index = i;
			timer.ticks_to_signal = 1000000; //TODO: Change this so it can be modified easily.

			//Init timer mutex and cond signal
			pthread_cond_init(&timer.timer_tick,0);
			pthread_mutex_init(&timer.timer_mutex,NULL);

			struct timer *my_timer = malloc(sizeof(struct timer));
			*my_timer = timer;

			strcpy(g_machine.cpu_ptr[i].threads[1].proc_name, "PGNR-TIMER");	
			pthread_create(&g_machine.cpu_ptr[i].threads[1].thread_handle,NULL,(void*)timer_routine,my_timer); //Timer

			strcpy(g_machine.cpu_ptr[i].threads[2].proc_name, "PGNR");	
			pthread_create(&g_machine.cpu_ptr[i].threads[2].thread_handle, NULL, (void *) process_generator_routine, my_timer); //Process Generator
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
