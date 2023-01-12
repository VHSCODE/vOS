//
// Created by v on 14/12/22.
//

#ifndef VOS_INSTRUCTIONS_H
#define VOS_INSTRUCTIONS_H


#include "CPU.h"

extern void ADD(struct thread* thread, u32 dest, u32 a, u32 b);
extern void ST(struct thread* thread,  u32 reg, u32 vAddr);
extern void LD(struct thread* thread, u32 reg, u32 vAddr);
#endif // VOS_INSTRUCTIONS_H
