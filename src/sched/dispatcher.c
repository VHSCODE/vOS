#include "dispatcher.h"
#include "../core/CPU/CPU.h"
#include "string.h"
#include <stdio.h>

u32 dispatch(u32 cpu_index, struct pcb *pcb)
{
    int i,j;

    pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].dispatcher_mutex);

    for(i=0; i < g_machine.cpu_ptr[cpu_index].core_count;i++){
        for(j=0; j < MAX_THREADS_PER_CORE; j++){
            if(g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].is_available == 1){ //Thread available
                //Context switch


                g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].pcb_ptr = pcb;

                g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].tcb.thread_program_counter =
                    pcb->state.proc_program_counter;

                memcpy(g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].tcb.thread_m_registers, pcb->state.proc_registers, 32 * sizeof(u32));

                pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].dispatcher_mutex);
                printf("Process with PID=%d (%s) and Priority=%d assigned to core/thread (%d,%d) in CPU %d\n", pcb->pid, pcb->filename,pcb->priority,i,j,cpu_index);
                g_machine.cpu_ptr[cpu_index].cpu_cores[i].threads[j].is_available = 0;
                return DISPATCH_SUCCESS;
            }

        }
    }
    pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].dispatcher_mutex);

    return 1;
}