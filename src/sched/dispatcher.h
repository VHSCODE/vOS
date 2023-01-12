#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "pcb.h"
#define DISPATCH_SUCCESS 0
extern u32 dispatch(u32 cpu_index, struct pcb *pcb);

#endif //DISPATCHER_H