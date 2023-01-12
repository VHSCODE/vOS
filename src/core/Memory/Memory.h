#ifndef VOS_MEMORY_H
#define VOS_MEMORY_H

#include "../../types.h"

#include  "../../sched/pcb.h"
#define MEM_SIZE 16777216 //2^24 -> 0xFFFFFF in Hex

//Base address of the kernel.
#define KERNEL_BASE 0x000000
#define KERNEL_SIZE 0x500000 // 0x500000 * 4 Bytes = 20 MiB

//Location of the page table within the Kernel memory region
#define PAGE_TABLE_BASE 0x000500

//Size of the page
#define PAGE_SIZE 4096 // 4KB
#define PAGE_COUNT 1048576 // 2^20

#define PAGE_TABLE_SIZE PAGE_COUNT * 4 // 4MiB

//Used to index the physical memory by frames
#define FRAME_STRIDE PAGE_SIZE / 4 //4096 B Frames / 4 B = 1024

//How many usable frames(discounting for those that are in kernel space)

#define FRAME_COUNT (MEM_SIZE - KERNEL_SIZE) / FRAME_STRIDE
#define FRAME_AVAILABLE 1



struct PhysicalMemory{
    u32 physical_memory[MEM_SIZE]; //2^24 B * 4 B = 64 MiB Memory
    u8 frame_bitmap[FRAME_COUNT]; //This counts the available frames starting FROM THE END OF THE KERNEL REGION

    u32 next_page_entry;
};

extern void Init_Memory();

extern void Write_Memory(u32 pAddr, u32 data);
extern u32 Read_Memory(u32 pAddr);

extern void Allocate_Page_Table_and_copy(u32 cpu_index, struct pcb *pcb);
extern void Free_PhysicalFrame(struct pcb *pcb);
#endif // VOS_MEMORY_H
