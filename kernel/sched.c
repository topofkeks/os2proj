//
// Created by aleksa on 12/11/21.
//

#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"
#include "sched.h"

struct proc *fifo[NPROC];
uint64 head = 0, tail = 0;

struct proc*
get()
{
    if (head == tail) return 0;
    struct proc *ret = fifo[head++];
    if (head == NPROC) head = 0;

    if (ret->burst_len && !ret->timeslice)
        ret->timeslice = ret->burst_len;
    return ret;
}

void
put(struct proc *p)
{
    if (!p) return;

    fifo[tail++] = p;
    if (tail == NPROC) tail = 0;
}
