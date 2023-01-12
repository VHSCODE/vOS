#include "MMU.h"
#include "Memory.h"
#include "stdio.h"
#include <stdlib.h>

u32 MMU(struct thread* thread,u32 vAddr,const u32 *pgb){

    //Check for TLB
    u32 addr = check_for_addr(&thread->TLB, vAddr);
    if(addr != MAGIC_NOT_FOUND)
        return addr; //Hit
    else{
        //Start translation and store in TLB

        u32 pAddr;
        u32 nPag;
        u32 offset;
        u32 baseFrame;

        //Translate
        offset = (vAddr & 0xFFF) / 4; //Extract lower 12 bits and divide by 4 ( every address is a 4 byte data value)
        baseFrame = pgb[0]; //Get base frame from page table entry

        pAddr = (baseFrame * FRAME_STRIDE) + KERNEL_SIZE  + offset; //Append the baseFrame and offset;

        if(pAddr <= (KERNEL_BASE + KERNEL_SIZE) - 1){ //Not permitted. Abort
            printf("KERNEL READ/WRITE DETECTED. ABORTING\n");
            exit(-1);
        }

        add_to_tlb(&thread->TLB, vAddr,pAddr);

        return pAddr;
    }

}

