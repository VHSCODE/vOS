#include "process_generator.h"
#include "machine.h"
#include "time.h"
void process_generator_routine(struct timer *my_timer){

    srand(time(NULL));
    

    while(g_machine.is_running){


        pthread_mutex_lock(&my_timer->timer_mutex);
        pthread_cond_wait(&my_timer->timer_tick,&my_timer->timer_mutex);

                //TODO hacer algo
        // if tick del timer, entonces creamos un proceso con emit_process
        
        //printf("Emit process\n");

        pthread_mutex_unlock(&my_timer->timer_mutex);

    }
}


void emit_process(){
    

    struct pcb pcb;

    pcb.pid = rand();
    
    
    //TODO annadir proceso a la cola de procesos
}
