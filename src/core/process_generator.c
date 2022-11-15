#include "process_generator.h"
#include "machine.h"
#include "time.h"

#include "stdio.h"
#include "stdlib.h"

#include "../sched/pcb.h"
void emit_process(u32 cpu_index);
void process_generator_routine(struct thread *thr){

    srand(time(NULL));
    

    while(g_machine.is_running){


        pthread_mutex_lock(&thr->timer->timer_mutex);
        pthread_cond_wait(&thr->timer->timer_tick,&thr->timer->timer_mutex);

        // if tick del timer, entonces creamos un proceso con emit_process
        
        printf("Generando proceso\n");

		emit_process(thr->timer->cpu_index);
        pthread_mutex_unlock(&thr->timer->timer_mutex);

    }
}


void emit_process(u32 cpu_index){
    
    //Aqui creamos una pcb de forma aleatoria
	struct pcb* pcb = malloc(sizeof (struct pcb));


    //FIXME: Habria que conseguir un pid unico

    pcb->pid = rand() % INT16_MAX;
    pcb->priority = rand() % PRIORITY_COUNT;

    append_to_queue(pcb,cpu_index);
}
