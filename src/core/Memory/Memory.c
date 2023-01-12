
#include "Memory.h"
#include <stdlib.h>
#include <string.h>

#include "../../Machine.h"
#include "../CPU/CPU.h"
#include "stdio.h"
void Init_Memory(){

    //Feature: Might add ASLR in the future
    //g_machine.p_memory.kernel_base = KERNEL_BASE;



    memset(g_machine.p_memory.physical_memory,0, sizeof(u32) * MEM_SIZE);
    memset(g_machine.p_memory.frame_bitmap,FRAME_AVAILABLE, sizeof(u8) * FRAME_COUNT);

    g_machine.p_memory.physical_memory[(KERNEL_BASE + KERNEL_SIZE)-1] = 0x4B1D;//Mark the end of the kernel region for debug
}
void Write_Memory(u32 pAddr, u32 data)
{
    g_machine.p_memory.physical_memory[pAddr] = data;
}
u32 Read_Memory(u32 pAddr)
{
    return g_machine.p_memory.physical_memory[pAddr];
}
void Allocate_Page_Table_and_copy(u32 cpu_index, struct pcb *pcb)
{

    pthread_mutex_lock(&g_machine.cpu_ptr[cpu_index].pagetable_mutex);

    //First, check for available frame in physical memory
    int frame;
    for(frame = 0; frame < FRAME_COUNT; frame++){
        if(g_machine.p_memory.frame_bitmap[frame] == FRAME_AVAILABLE){
            g_machine.p_memory.frame_bitmap[frame] = 0; //Mark as occupied
            //We assume that every pair of .text and .data segments can be stored in the same frame.

            //Allocate page table entry
            g_machine.p_memory.physical_memory[PAGE_TABLE_BASE + g_machine.p_memory.next_page_entry] = frame;

            pcb->mm.pgb = &g_machine.p_memory.physical_memory[PAGE_TABLE_BASE + g_machine.p_memory.next_page_entry];

            g_machine.p_memory.next_page_entry++;

            //Copy to physical memory
            //.text
            memcpy(&g_machine.p_memory.physical_memory[(frame * FRAME_STRIDE)+ KERNEL_SIZE],pcb->mm.code,sizeof(u32)* pcb->mm.code_size);

            //.data
            memcpy(&g_machine.p_memory.physical_memory[(frame * FRAME_STRIDE)+ KERNEL_SIZE + pcb->mm.code_size],pcb->mm.data,sizeof(u32)* pcb->mm.data_size);

            pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].pagetable_mutex);
            return;
        }
    }

    pthread_mutex_unlock(&g_machine.cpu_ptr[cpu_index].pagetable_mutex);

    printf("ALL FRAMES ARE FILLED. Aborting...\n"); //FIXME implemente a page table replacement system
    exit(-1);
}
void Free_PhysicalFrame(struct pcb *pcb)
{
    //Zero used memory
    g_machine.p_memory.frame_bitmap[pcb->mm.pgb[0]] = FRAME_AVAILABLE;

}
