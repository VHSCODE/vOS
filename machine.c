//
// Created by v on 30/09/22.
//

#include "machine.h"
#include "stdlib.h"

void init_machine(u32 cpu_count, struct machine * machine){

	machine->cpu_count = cpu_count;

	machine->cpu_ptr = calloc(cpu_count,sizeof(struct cpu));
	int i;

}


void deinit_machine(struct machine* machine){

	//TODO also free the pointers inside the cpu_ptr
	free(machine->cpu_ptr);
}