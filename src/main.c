#include "Machine.h"
#include <stdlib.h>

#include "stdio.h"
#include "unistd.h"



int main(int argc, char **argv)
{
   
    //Default specs
    struct machine_specs specs;

    specs.core_count = 8;
    specs.cpu_count = 1;
    specs.threads_per_core = 2;
    specs.sched_frec = 100000 / 2;
    specs.procgnr_frec = 10000;
    
    
    // Parse args

	if(argc == 3){
		specs.sched_frec = atol(argv[1]);
		specs.procgnr_frec = atol(argv[2]);
	}

	Init_Machine(specs); // Iniciamos nuestra maquina global

    while (g_machine.is_running)
    {

    }

	Deinit_Machine();
    return 0;
}
