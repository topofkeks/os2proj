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
#include "sjf.h"

struct spinlock schedlock;
struct proc *(*getter)(void) = sjf_get;
void(*putter)(struct proc *) = sjf_put;


void
set_sched(struct proc *(*get)(void), void(*put)(struct proc *))
{
  getter = get;
  putter = put;
}

struct proc*
get()
{
    acquire(&schedlock);
    struct proc *ret = (*getter)();
    release(&schedlock);
    return ret;
}

void
put(struct proc *p)
{
    if (!p) panic("sched: put null");
    if (p->state != RUNNABLE) panic("put non runnable");

    acquire(&schedlock);
    (*putter)(p);
    release(&schedlock);
}
