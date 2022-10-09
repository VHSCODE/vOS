//
// Created by v on 30/09/22.
//

#include "stdlib.h"
#include <pthread.h>

#include "machine.h"
#include "timer.h"
#include "process_generator.h"
#include "clock.h"


#define BASE_THREADS 3 //One cpu clock, one timer and one process_generator
#define TIMER_COUNT 1 
struct machine g_machine;

void init_machine(u32 cpu_count){

	g_machine.cpu_count = cpu_count;

	g_machine.cpu_ptr = calloc(cpu_count,sizeof(struct cpu));

	int i;
	
	g_machine.is_running = 1;

	//Allocate and start  threads per cpu
	for ( i = 0; i < g_machine.cpu_count; i++)
	{
		g_machine.cpu_ptr[i].threads = calloc(BASE_THREADS + (TIMER_COUNT - 1),sizeof(struct thread)); //Decrement the timer count to add the non required timers
		g_machine.cpu_ptr[i].thread_count = BASE_THREADS + (TIMER_COUNT - 1);

		//Init condition variables
		pthread_cond_init(&g_machine.cpu_ptr[i].clock_tick,0);

		pthread_mutex_init(&g_machine.cpu_ptr[i].clock_mutex,NULL);

		//Init threads for every routine
		pthread_create(&g_machine.cpu_ptr[i].threads[0].thread_handle, NULL, (void*)clock_routine, i); //Clock
		int j = 0;
		//Init timers per cpu
		for(j; j < TIMER_COUNT; j++){ //https://stackoverflow.com/questions/39300744/how-to-pass-a-struct-by-value-to-a-pthread
			
			struct timer timer;
			timer.timer_thread_index = j+1;
			timer.cpu_index = i;
			timer.ticks_to_signal = 10000; //TODO: Change this so it can be modified easily.


			struct timer *my_timer = malloc(sizeof(struct timer));
			*my_timer = timer;

			pthread_create(&g_machine.cpu_ptr[i].threads[j+1].thread_handle,NULL,(void*)timer_routine,my_timer); //Timer
		}
		
		pthread_create(&g_machine.cpu_ptr[i].threads[j+1].thread_handle, NULL, (void *) process_generator_routine, NULL); //Process Generator

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
	//Allocate and start  threads per cpu
	for ( i = 0; i < g_machine.cpu_count; i++)
	{
		free(g_machine.cpu_ptr[i].threads);
	}

	free(g_machine.cpu_ptr);
}
