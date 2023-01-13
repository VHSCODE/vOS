
#ifndef CPU_H
#define CPU_H
#include "../../sched/pcb.h"
#include "../../types.h"
#include "pthread.h"
#include "../../Machine.h"
#include "../Memory/MMU.h"
#include "../Memory/TLB.h"
#define MAX_THREADS_PER_CORE 2

struct TCB
{
	u32 thread_program_counter;
    u32 thread_m_registers[32];
};

struct thread
{
	struct pcb *pcb_ptr;
	struct TCB tcb;
    u8 thread_id;
    u8 is_available;
    struct TLB TLB;
};





struct CPU_CORE{
    struct thread threads[MAX_THREADS_PER_CORE];



};
struct CPU{
    struct CPU_CORE * cpu_cores;
	u32 core_count;

	struct process_queue run_queue;

    //Queue lock
	pthread_cond_t queue_cond_lock;
	pthread_mutex_t queue_mutex_lock;

	//Global timer lock
	pthread_cond_t global_timer_lock;
	pthread_mutex_t global_timer_mutex;

    //Dispatcher lock
    pthread_mutex_t dispatcher_mutex;

    //PageTable access
    pthread_mutex_t pagetable_mutex;

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

extern void Init_CPU(u32 cpu_index,struct machine_specs specs);
extern void Init_CPU_Subsystems(u32 cpu_index,struct machine_specs specs);

extern u32 Read_From_Register(u32 cpu_index, u32 core_index,u32 reg);
extern void Write_To_Register(u32 cpu_index, u32 core_index,u32 reg, u32 data);
extern void Decode_And_Execute(u32 cpu_index, struct thread *thread);
extern void Step_Cpu(u32 cpu_index);

#endif