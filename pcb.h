#ifndef VOS__PCB_H_
#define VOS__PCB_H_

#include "types.h"

struct pcb {
	u32 pid;
};

struct Node{
    struct pcb pcb;
    struct Node *next;

};
struct pcb_queue
{
    struct Node *node;
    u32 size;
};


extern void append_to_queue(struct pcb new_pcb);
extern void swap_queues();
#endif
