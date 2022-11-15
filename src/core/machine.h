//
// Created by v on 30/09/22.
//

#ifndef VOS__MACHINE_H_
#define VOS__MACHINE_H_

#include "pthread.h"

#include "../types.h"
#include "../sched/pcb.h"

/**
 * Representa nuestra maquina , con sus cpus, threads y variables.
 */

extern struct machine g_machine; //Instancia global de nuestra maquina;

struct thread{
	struct pcb *pcb_ptr; //Usado en un futuro para tambien representar los programas en ejecucion
	pthread_t thread_handle;
	char proc_name[512];
	struct timer * timer;
};

struct cpu{
	u32 core_count;
	struct thread *threads;
	u32 thread_count;

	struct process_queue run_queue;
	pthread_cond_t queue_cond;
	pthread_mutex_t queue_mutex;


	//Cpu clock
	pthread_cond_t clock_tick;
	pthread_mutex_t clock_mutex;
	

};
struct machine{
	struct cpu *cpu_ptr;
	long cpu_count;

	u8 volatile is_running;

};


extern void init_machine(u32 cpu_count, u32 sched_frec, u32 procgnr_frec);
extern void deinit_machine();
extern void graceful_shutdown();


#endif //VOS__MACHINE_H_
