#ifndef VOS__PCB_H_
#define VOS__PCB_H_

#include "types.h"

#define PRIORITY_COUNT 120
#define QUEUE_SIZE 100

struct pcb {
	u32 pid;
	u32 priority;
};

struct node{
	struct pcb* pcb;
	struct node* next;
};

struct queue{

	u8 bitmap[PRIORITY_COUNT];
	struct node* queue_ptr[PRIORITY_COUNT];
};
struct process_queue{

	struct queue* normal_queue;

	struct queue* expired_queue;

};

extern void append_to_queue(struct pcb* new_pcb,u32 cpu_index);

extern void delete_last_pcb(u32 priority,u32 cpu_index);
extern struct pcb* get_last_pcb(u32 priority, u32 cpu_index);

extern void swap_queues(u32 cpu_index);




#endif
