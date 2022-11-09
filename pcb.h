#ifndef VOS__PCB_H_
#define VOS__PCB_H_

#include "types.h"

#define PRIORITY_COUNT 120
#define QUEUE_SIZE 100

struct pcb {
	u32 pid;
};

struct node{
	struct pcb* pcb;
	struct node* next;
};

struct  queue{

	u8 bitmap[PRIORITY_COUNT];
	struct node* proc_ptr[PRIORITY_COUNT];
};

struct process_queue{

	struct queue* normal_queue;
	struct queue* expired_queue;
};




#endif
