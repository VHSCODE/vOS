//
// Created by v on 30/09/22.
//

#include "stdlib.h"
#include <pthread.h>

#include "machine.h"
#include "timer.h"
#include "process_generator.h"
#include "clock.h"


#define THREADS 3 
struct machine g_machine;

void init_machine(u32 cpu_count){

	g_machine.cpu_count = cpu_count;

	g_machine.cpu_ptr = calloc(cpu_count,sizeof(struct cpu));

	int i;
	//Allocate and start  threads per cpu
	for ( i = 0; i < g_machine.cpu_count; i++)
	{
		g_machine.cpu_ptr[i].threads = calloc(THREADS,sizeof(struct thread));
		g_machine.cpu_ptr[i].thread_count = THREADS;

		//Init threads for every routine
		pthread_create(&g_machine.cpu_ptr[i].threads[0], NULL, clock_routine, NULL); //Clock
		pthread_create(&g_machine.cpu_ptr[i].threads[1],NULL,timer_routine,NULL); //Timer
		pthread_create(&g_machine.cpu_ptr[i].threads[2], NULL, process_generator_routine, NULL); //Process Generator

	}
	
	g_machine.is_running = 1;

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
			pthread_join(&g_machine.cpu_ptr[i].threads[j],NULL);
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

	//TODO also free the pointers inside the cpu_ptr
	free(g_machine.cpu_ptr);
}
