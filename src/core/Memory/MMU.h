//
// Created by v on 13/12/22.
//

#ifndef VOS_MMU_H
#define VOS_MMU_H

#include "../../types.h"
#include "../CPU/CPU.h"
 extern u32 MMU(struct thread* thread, u32 vAddr,const u32* pgb);

#endif // VOS_MMU_H
