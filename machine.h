//
// Created by v on 30/09/22.
//

#ifndef VOS__MACHINE_H_
#define VOS__MACHINE_H_


#include "types.h"
/**
 * Represents our virtual Machine, along with all its cpus and their respective threads
 */

struct thread{
	struct pcb *pcb_ptr;
	struct cpu *p_cpu_ptr; //Parent cpu pointer
};

struct cpu{
	u32 core_count;
	struct thread *threads;
	u32 thread_count;

};
struct machine{
	struct cpu *cpu_ptr;
	long cpu_count;

};
void init_machine(u32 cpu_count, struct machine * machine);
void deinit_machine(struct machine* machine);



#endif //VOS__MACHINE_H_
