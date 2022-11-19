//
// Created by v on 30/09/22.
//

#ifndef VOS__MACHINE_H_
#define VOS__MACHINE_H_

#include "pthread.h"

#include "types.h"
#include "sched/pcb.h"


extern struct Machine g_machine; //Instancia global de nuestra maquina;

struct machine_specs{
    
    u32 cpu_count;
    u32 core_count;
    u32 threads_per_core;

    long sched_frec;
    long procgnr_frec;
};

struct Machine{
	struct CPU *cpu_ptr;
	long cpu_count;
	u8 volatile is_running;

};

extern void Init_Machine(struct machine_specs specs);
extern void Deinit_Machine();

#endif //VOS__MACHINE_H_
