#include "Loader.h"
#include "time.h"

#include "stdio.h"
#include "stdlib.h"

void emit_process(u32 cpu_index);
void process_generator_routine(struct Timer *timer){

    srand(time(NULL));
    

    while(g_machine.is_running){


        pthread_mutex_lock(&timer->timer_mutex);
        pthread_cond_wait(&timer->timer_tick,&timer->timer_mutex);

        // if tick del Timer, entonces creamos un proceso con emit_process
        
        printf("Generando proceso\n");

		emit_process(timer->cpu_index);
        pthread_mutex_unlock(&timer->timer_mutex);

    }
    free(timer);
}


void emit_process(u32 cpu_index){
    
    //Aqui creamos una pcb de forma aleatoria
	struct pcb* pcb = malloc(sizeof (struct pcb));


    //FIXME: Habria que conseguir un pid unico

    pcb->pid = rand() % INT16_MAX;
    pcb->priority = rand() % PRIORITY_COUNT;

    append_to_queue(pcb,cpu_index);
}
