//
// Created by v on 4/01/23.
//

#ifndef VOS_TLB_H
#define VOS_TLB_H
#include "../../types.h"

#define MAX_TLB_SIZE 32
#define MAGIC_NOT_FOUND 0xdeadbeef

struct TLBEntry{
    u32 tag;
    u32 translation;
};

struct TLB{
    struct TLBEntry entries[MAX_TLB_SIZE];
    u32 size;
    u32 next_to_exit;

};

extern u32 check_for_addr(struct TLB *tlb, u32 vAddr);
extern void add_to_tlb(struct TLB *tlb, u32 vAddr, u32 pAddr);

#endif // VOS_TLB_H
