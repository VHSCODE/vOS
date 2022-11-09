#include "pcb.h"
#include "pthread.h"
#include "machine.h"
void append_to_queue(struct pcb new_pcb,u32 cpu_index)
{
    //pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].normal_queue_mutex);
    
    //Para escribirlo mas facil
    //struct pcb_queue *queue = g_machine.cpu_ptr[cpu_index].run_queue->normal_queue;


    return;
}