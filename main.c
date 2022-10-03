#include <stdio.h>
#include "pthread.h"

#include "machine.h"

int main()
{

	struct machine machine;

	init_machine(1,&machine);

	int running = 1;
	machine_deinit(&machine);
}
