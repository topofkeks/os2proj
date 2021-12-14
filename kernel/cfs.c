//
// Created by aleksa on 12/14/21.
//

#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"
#include "heap.h"

uint8 cfs_active = 1;

struct proc*
cfs_get(void)
{
    if (!cfs_active) panic("cfs inactive");
    uint64 current = 0;

    if (!tickslock.locked){
        acquire(&tickslock);
        current = ticks;
        release(&tickslock);
    } else current = ticks;

    struct proc *ret = heap_pop();
    ret->timeslice = (current - ret->last_put) / NCPU;
    if (ret->timeslice == 0) ret->timeslice = 1;
    return ret;
}

void
cfs_put(struct proc *p)
{
    if (!cfs_active) panic("cfs inactive");
    if (!p) return;

    if (!tickslock.locked){
        acquire(&tickslock);
        p->last_put = ticks;
        release(&tickslock);
    } else p->last_put = ticks;

    heap_push(p);
}

uint8
cfs_proc_lt(struct proc* a, struct proc* b)
{
    if (a && b && a->burst_length < b->burst_length) return 1;
    return 0;
}
