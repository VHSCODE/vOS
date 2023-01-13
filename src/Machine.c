//
// Created by v on 30/09/22.
//

#include "stdlib.h"
#include <stdio.h>

#include "Machine.h"
#include "core/CPU/CPU.h"
#include "core/Memory/Memory.h"

struct Machine g_machine;

void Init_Machine(struct machine_specs specs)
{
    g_machine.specs = specs;
    g_machine.cpu_count = g_machine.specs.cpu_count;
    g_machine.cpu_ptr = calloc(g_machine.specs.cpu_count, sizeof(struct CPU));

	g_machine.is_running = 1;

    Init_Memory();

    int i;
    // Allocate and start  threads per cpu core
    for (i = 0; i < g_machine.cpu_count; i++)
    {
		Init_CPU(i,specs);
		Init_CPU_Subsystems(i,specs);

    }

}
void Deinit_Machine()
{
    printf("Started Cleanup\n");
    //Join threads

    /*
    int i;
    for(i = 0; g_machine.cpu_count; i++){
        //Join subsystem threads

        //Scheduler
        pthread_join(g_machine.cpu_ptr[i].scheduler_timer_handle,NULL);
        printf("Scheduler Timer thread joined\n");
        pthread_join(g_machine.cpu_ptr[i].scheduler_handle,NULL);


        //Loader
        pthread_join(g_machine.cpu_ptr[i].loader_timer_handle,NULL);
        pthread_join(g_machine.cpu_ptr[i].loader_handle,NULL);
        printf("Loaders threads joined\n");


        //Clock
        pthread_join(g_machine.cpu_ptr[i].clock_handle,NULL);
        printf("Clock thread joined\n");

    }
*/
    //Start freeing resources
    free(g_machine.cpu_ptr);

    printf("Cleanup Complete. Bye !\n");

}
