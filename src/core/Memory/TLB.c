#include "TLB.h"


u32 check_for_addr(struct TLB *tlb, u32 vAddr) //Naive approach. A hashmap/hashset should work better.
{
    int i = 0;
    for(i; i <tlb->size;i++)
    {
        if(tlb->entries[i].tag == vAddr){
            return tlb->entries[i].translation;
        }
    }
    return MAGIC_NOT_FOUND;
}
void add_to_tlb(struct TLB *tlb, u32 vAddr, u32 pAddr)
{
    if(tlb->size >= MAX_TLB_SIZE){ //Full. Start deleting the oldest ones

        tlb->entries[tlb->next_to_exit].tag = vAddr;
        tlb->entries[tlb->next_to_exit].translation = pAddr;
        tlb->next_to_exit = tlb->next_to_exit < MAX_TLB_SIZE ? tlb->next_to_exit+1 : 0;
    }
    else{ //Store the translation as normal.
        tlb->entries[tlb->size].tag = vAddr;
        tlb->entries[tlb->size].translation = pAddr;
        tlb->size++;

    }
}
