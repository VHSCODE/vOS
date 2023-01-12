#ifndef VOS__PCB_H_
#define VOS__PCB_H_

#include "../types.h"

#define PRIORITY_COUNT 120
#define MAX_CYCLES_ALLOWED 10
#define MAX_INSTRUCTIONS 256
#define MAX_DATA 512

struct count_stats{
	
	u32 cpu_cycles_run;
    u32 cycles_allowed; //1 Cycle = 1 Instruction
};

struct segments{
    u32 code[MAX_INSTRUCTIONS];
    u32 code_size;

    u32 data [MAX_DATA];
    u32 data_size;

    u32 * pgb;

};

struct proc_state{
    u32 proc_program_counter;
    u32 proc_registers[32];
};
struct pcb {
	u32 pid;
	u32 priority;

    struct count_stats counting;
    struct segments mm;

    struct proc_state state;

    char filename[50];
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

extern void append_to_normal_queue(struct pcb* new_pcb,u32 cpu_index);
extern void append_to_expired_queue(struct pcb* new_pcb,u32 cpu_index);


extern void delete_last_pcb(u32 priority,u32 cpu_index);
extern struct pcb* get_last_pcb(u32 priority, u32 cpu_index);

extern void swap_queues(u32 cpu_index);




#endif
