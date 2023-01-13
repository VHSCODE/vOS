

#include "CPU.h"
#include <stdio.h>
#include <string.h>

#include "stdlib.h"

#include "../../sched/scheduler.h"
#include "../Memory/Loader.h"
#include "Clock.h"
#include "Instructions.h"
#include "Timer.h"

void Init_CPU(u32 cpu_index,struct machine_specs specs)
{
	g_machine.cpu_ptr[cpu_index].cpu_cores = malloc(specs.core_count * sizeof(struct CPU_CORE));
	g_machine.cpu_ptr[cpu_index].core_count = g_machine.specs.core_count;

	g_machine.cpu_ptr[cpu_index].run_queue.normal_queue = malloc(sizeof(struct queue));
	memset(g_machine.cpu_ptr[cpu_index].run_queue.normal_queue->bitmap, 0, PRIORITY_COUNT);

	g_machine.cpu_ptr[cpu_index].run_queue.expired_queue = malloc(sizeof(struct queue));
	memset(g_machine.cpu_ptr[cpu_index].run_queue.expired_queue->bitmap, 0, PRIORITY_COUNT);

	pthread_cond_init(&g_machine.cpu_ptr[cpu_index].queue_cond_lock, 0);
	pthread_mutex_init(&g_machine.cpu_ptr[cpu_index].queue_mutex_lock, NULL);

    pthread_mutex_init(&g_machine.cpu_ptr[cpu_index].dispatcher_mutex, NULL);
    pthread_mutex_init(&g_machine.cpu_ptr[cpu_index].pagetable_mutex, NULL);



    //Init threads

    int i,j;
    int thread_id = 0;
    for(i = 0; i < g_machine.cpu_ptr[cpu_index].core_count; i++){
        for(j = 0; j< MAX_THREADS_PER_CORE; j++){
            g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].is_available = 1;
            g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].tcb.thread_program_counter = 0x0;
            memset(&g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].tcb.thread_m_registers,0,sizeof(u32) *32);
            g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].thread_id = thread_id;
            thread_id++;


        }
    }

}

void Init_CPU_Subsystems(u32 cpu_index,struct machine_specs specs)
{
	//CPU CLOCK
	pthread_cond_init(&g_machine.cpu_ptr[cpu_index].clock_tick, 0);
	pthread_mutex_init(&g_machine.cpu_ptr[cpu_index].clock_mutex, NULL);
	pthread_cond_init(&g_machine.cpu_ptr[cpu_index].global_timer_lock, 0);
	pthread_mutex_init(&g_machine.cpu_ptr[cpu_index].global_timer_mutex, NULL);
	pthread_create(&g_machine.cpu_ptr[cpu_index].clock_handle, NULL, (void *)clock_routine, (void *)cpu_index); // Clock

	

	//LOADER
	struct Timer *loader_timer = malloc(sizeof(struct Timer));

	loader_timer->cpu_index = cpu_index;
	loader_timer->ticks_to_signal = g_machine.specs.procgnr_frec;

	pthread_cond_init(&loader_timer->timer_tick, 0);
	pthread_mutex_init(&loader_timer->timer_mutex, 0);

	pthread_create(&g_machine.cpu_ptr[cpu_index].loader_timer_handle, NULL, (void *)timer_routine, loader_timer); // Timer

	pthread_create(&g_machine.cpu_ptr[cpu_index].loader_handle, NULL, (void *)Loader_Routine, loader_timer); // Process Generator


	//SCHEDULER
	struct Timer *scheduler_timer = malloc(sizeof(struct Timer));

	scheduler_timer->cpu_index = cpu_index;
	scheduler_timer->ticks_to_signal = g_machine.specs.sched_frec;

	pthread_cond_init(&scheduler_timer->timer_tick, 0);
	pthread_mutex_init(&scheduler_timer->timer_mutex, 0);

	pthread_create(&g_machine.cpu_ptr[cpu_index].scheduler_timer_handle, NULL, (void *)timer_routine,
		scheduler_timer); // Timer del scheduler


	pthread_create(&g_machine.cpu_ptr[cpu_index].scheduler_handle, NULL, (void *)scheduler_routine,scheduler_timer); // Scheduler


}

void Decode_And_Execute(u32 cpu_index, struct thread *thread)
{

    u32 pAddr = MMU(thread,thread->tcb.thread_program_counter,thread->pcb_ptr->mm.pgb);
    u32 instr = Read_Memory(pAddr) ;
    u32 opcode = instr >> 28; //Extract operation code, last 4 bits of the instruction

    thread->tcb.thread_program_counter+=4;

    switch (opcode)
    {
    case 0x0: {
        // LD
        u32 dest = (instr >> 24) & 0xF;
        u32 vAddr = instr & 0x00FFFFFF;
        LD(thread, dest, vAddr);
    }
        break;
    case 0x1: {
        // ST
        u32 reg = (instr >> 24) & 0xF;
        u32 vAddr = instr & 0x00FFFFFF;

        ST(thread,reg,vAddr);
        }
        break;
    case 0x2: {
        // ADD
        u32 dest = (instr >> 24) & 0xF;
        u32 rd1 = (instr >> 20) & 0xF;
        u32 rd2 = (instr >> 16) & 0xF;

        ADD(thread, dest, rd1, rd2);
    }
        break;
    case 0xF:  //Exit (stop)
        printf("Process %d is finished.\n",thread->pcb_ptr->pid);
        //Mark used frame as available
        Free_PhysicalFrame(thread->pcb_ptr);

        free(thread->pcb_ptr);
        thread->pcb_ptr = NULL; //Scary
        thread->is_available = 1;
        return;

    default:
        break;
    }
    thread->pcb_ptr->counting.cpu_cycles_run++;
    if(thread->pcb_ptr->counting.cpu_cycles_run >= thread->pcb_ptr->counting.cycles_allowed){ //Can't execute more for the time being. Move it to the expired queue and mark thread as available

        printf("Process %d is not allowed to execute more. Moving to expired queue\n",thread->pcb_ptr->pid);
        //Update fields
        thread->pcb_ptr->state.proc_program_counter = thread->tcb.thread_program_counter;
        //Copy register values to pcb.
        memcpy(thread->pcb_ptr->state.proc_registers,thread->tcb.thread_m_registers, 32 * sizeof(u32));
        thread->pcb_ptr->counting.cpu_cycles_run =0;

        append_to_expired_queue(thread->pcb_ptr,cpu_index);

        thread->is_available = 1;
        thread->pcb_ptr = NULL; //Scary
    }
}
void Step_Cpu(u32 cpu_index)
{
    //Execute instruction per hardware thread
    int i,j;
    for (i=0; i < g_machine.cpu_ptr[cpu_index].core_count; i++)
    {
        for (j = 0; j < MAX_THREADS_PER_CORE ;j++)
        {
            if(!g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].is_available)
                Decode_And_Execute(cpu_index, &g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j]);
        }
    }
}
