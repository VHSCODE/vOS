//
// Created by v on 30/09/22.
//

#ifndef VOS__MACHINE_H_
#define VOS__MACHINE_H_

#include "pthread.h"

#include "types.h"
#include "pcb.h"
/**
 * Represents our virtual Machine, along with all its cpus and their respective threads
 */

extern struct machine g_machine; //Global instance of our machine;

struct thread{
	struct pcb *pcb_ptr;
	pthread_t thread_handle;
	//struct cpu *p_cpu_ptr; //Parent cpu pointer
};

struct cpu{
	u32 core_count;
	struct thread *threads;
	u32 thread_count;


	struct pcb_queue *normal_queue;
	struct pcb_queue *expired_queue;

	//Cpu clock
	pthread_cond_t clock_tick;
	pthread_mutex_t clock_mutex;
	

};
struct machine{
	struct cpu *cpu_ptr;
	long cpu_count;

	u8 volatile is_running;

};
extern void init_machine(u32 cpu_count);
extern void deinit_machine();
extern void graceful_shutdown();


#endif //VOS__MACHINE_H_
