
#ifndef CPU_H
#define CPU_H
#include "../types.h"
#include "../sched/pcb.h"
#include "pthread.h"

#include "../Machine.h"


#define MAX_THREADS_PER_CORE 2

struct thread_context{

	u32 program_counter;
};

struct thread
{
	struct pcb *pcb_ptr;

	struct thread_context ctx;
	u8 is_available;

	//Annadir tlb y MMU
};


struct CPU_CORE{
    u32 m_registers[32];

    u32 m_program_counter;
    
    struct thread threads[MAX_THREADS_PER_CORE];    

};
struct CPU{
    struct CPU_CORE * cpu_cores;
	u32 core_count;

	struct process_queue run_queue;
	pthread_cond_t queue_cond;
	pthread_mutex_t queue_mutex;


	pthread_cond_t global_timer_lock;


	//Cpu clock
	pthread_t clock_handle;
	pthread_cond_t clock_tick;
	pthread_mutex_t clock_mutex;

	//Scheduler
	pthread_t scheduler_handle;
	pthread_t scheduler_timer_handle;
	

	//Loader
	pthread_t loader_handle;
	pthread_t loader_timer_handle;
};

extern void Init_CPU(u32 cpu_index, struct machine_specs specs);
extern void Init_CPU_Subsystems(u32 cpu_index,struct machine_specs specs);
extern u32 Read_From_Register(u32 cpu_index, u32 core_index,u32 reg);
extern void Write_To_Register(u32 cpu_index, u32 core_index,u32 reg, u32 data);

#endif