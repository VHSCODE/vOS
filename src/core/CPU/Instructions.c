#include "Instructions.h"
void ADD(struct thread *thread, u32 dest,u32 a, u32 b)
{
    //Cast to i32 to retain numerical signs
    thread->tcb.thread_m_registers[dest] =  (i32) thread->tcb.thread_m_registers[a] + (i32)thread->tcb.thread_m_registers[b];
}
void ST(struct thread *thread, u32 reg, u32 vAddr)
{
    u32 pAddr = MMU(thread,vAddr,thread->pcb_ptr->mm.pgb);
    Write_Memory(pAddr,thread->tcb.thread_m_registers[reg]);
}
void LD(struct thread *thread, u32 reg, u32 vAddr)
{
    u32 pAddr = MMU(thread,vAddr,thread->pcb_ptr->mm.pgb);
    thread->tcb.thread_m_registers[reg] = Read_Memory(pAddr);
}
