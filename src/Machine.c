//
// Created by v on 30/09/22.
//

#include "stdlib.h"

#include "Machine.h"
#include "core/CPU.h"

struct Machine g_machine;

void Init_Machine(struct machine_specs specs)
{

    g_machine.cpu_count = specs.cpu_count;
    g_machine.cpu_ptr = calloc(specs.cpu_count, sizeof(struct CPU));

	g_machine.is_running = 1;

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

    free(g_machine.cpu_ptr);
}
