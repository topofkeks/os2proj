//
// Created by aleksa on 12/14/21.
//

#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

uint8 cfs_active = 1;

void
cfs_set(uint8 status){
  cfs_active = status;
}

struct proc*
cfs_get(void)
{
    if (!cfs_active) panic("cfs inactive");


    struct proc *ret = heap_pop();
    if (!ret) return ret;

    ret->timeslice = (ticks - ret->last_put) / NCPU;
    if (ret->timeslice == 0) ret->timeslice = 1;
    return ret;
}

void
cfs_put(struct proc *p)
{
    if (!cfs_active) panic("cfs inactive");
    if (!p) return;

    p->last_put = ticks;
    heap_push(p);
}

uint8
cfs_proc_lt(struct proc* a, struct proc* b)
{
    if (a->burst_length < b->burst_length) return 1;
    return 0;
}
