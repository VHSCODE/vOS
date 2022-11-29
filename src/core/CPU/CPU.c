

#include <string.h>
#include "CPU.h"

#include "../Machine.h"
#include "stdlib.h"

#include "Clock.h"
#include "Timer.h"
#include "Loader.h"

#include "../sched/scheduler.h"

void Init_CPU(u32 cpu_index,struct machine_specs specs)
{
	g_machine.cpu_ptr[cpu_index].cpu_cores = malloc(specs.threads_per_core * sizeof(struct CPU_CORE));
	g_machine.cpu_ptr[cpu_index].core_count = specs.core_count;

	g_machine.cpu_ptr[cpu_index].run_queue.normal_queue = malloc(sizeof(struct queue));
	memset(g_machine.cpu_ptr[cpu_index].run_queue.normal_queue->bitmap, 0, PRIORITY_COUNT);

	g_machine.cpu_ptr[cpu_index].run_queue.expired_queue = malloc(sizeof(struct queue));
	memset(g_machine.cpu_ptr[cpu_index].run_queue.expired_queue->bitmap, 0, PRIORITY_COUNT);

	pthread_cond_init(&g_machine.cpu_ptr[cpu_index].queue_cond, 0);
	pthread_mutex_init(&g_machine.cpu_ptr[cpu_index].queue_mutex, NULL);
}

void Init_CPU_Subsystems(u32 cpu_index,struct machine_specs specs)
{
	//CPU CLOCK
	pthread_cond_init(&g_machine.cpu_ptr[cpu_index].clock_tick, 0);
	pthread_mutex_init(&g_machine.cpu_ptr[cpu_index].clock_mutex, NULL);
	pthread_cond_init(&g_machine.cpu_ptr[cpu_index].global_timer_lock, 0);
	pthread_mutex_init(&g_machine.cpu_ptr[cpu_index].global_timer_mutex, NULL);
	pthread_create(&g_machine.cpu_ptr[cpu_index].clock_handle, NULL, (void *)clock_routine,cpu_index); // Clock

	

	//LOADER
	struct Timer *loader_timer = malloc(sizeof(struct Timer));

	loader_timer->cpu_index = cpu_index;
	loader_timer->ticks_to_signal = specs.procgnr_frec;

	pthread_cond_init(&loader_timer->timer_tick, 0);
	pthread_mutex_init(&loader_timer->timer_mutex, 0);

	pthread_create(&g_machine.cpu_ptr[cpu_index].loader_timer_handle, NULL, (void *)timer_routine, loader_timer); // Timer

	pthread_create(&g_machine.cpu_ptr[cpu_index].loader_handle, NULL, (void *)process_generator_routine, loader_timer); // Process Generator


	//SCHEDULER
	struct Timer *scheduler_timer = malloc(sizeof(struct Timer));

	scheduler_timer->cpu_index = cpu_index;
	scheduler_timer->ticks_to_signal = specs.sched_frec;

	pthread_cond_init(&scheduler_timer->timer_tick, 0);
	pthread_mutex_init(&scheduler_timer->timer_mutex, 0);

	pthread_create(&g_machine.cpu_ptr[cpu_index].scheduler_timer_handle, NULL, (void *)timer_routine,
		scheduler_timer); // Timer del scheduler


	pthread_create(&g_machine.cpu_ptr[cpu_index].scheduler_handle, NULL, (void *)scheduler_routine,scheduler_timer); // Scheduler


}

u32 Read_From_Register(u32 cpu_index,u32 core_index, u32 reg)
{
	if(reg <=31)
		return g_machine.cpu_ptr[cpu_index].cpu_cores[core_index].m_registers[reg];
	return 0xFF;
}
void Write_To_Register(u32 cpu_index, u32 core_index, u32 reg, u32 data)
{
	if(reg <=31)
		g_machine.cpu_ptr[cpu_index].cpu_cores[core_index].m_registers[reg] = data;
}

