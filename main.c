#include "machine.h"

#include "stdio.h"
#include "unistd.h"

int main(int argc, char **argv)
{
    // Parse args
    int c;
    long sched_frec = 100000 / 2;
    long procgnr_frec = 10000;

	if(argc == 3){
		sched_frec = atol(argv[1]);
		procgnr_frec = atol(argv[2]);
	}
    init_machine(1,sched_frec,procgnr_frec); // Iniciamos nuestra maquina global

    while (g_machine.is_running)
    {

    }

    deinit_machine();
    return 0;
}
