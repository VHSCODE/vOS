//
// Created by v on 30/09/22.
//

#ifndef VOS__MACHINE_H_
#define VOS__MACHINE_H_

#include "pthread.h"

#include "types.h"
#include "sched/pcb.h"

#include "core/Memory/Memory.h"

extern struct Machine g_machine; //Instancia global de nuestra maquina;

struct machine_specs{
    
    u32 cpu_count;
    u32 core_count;

    long sched_frec;
    long procgnr_frec;

    long program_count;
};

struct Machine{
	struct CPU *cpu_ptr;
    struct PhysicalMemory p_memory;
	long cpu_count;
    u8 volatile is_running;

    struct machine_specs specs;



};

extern void Init_Machine(struct machine_specs specs);
extern void Deinit_Machine();

#endif //VOS__MACHINE_H_
