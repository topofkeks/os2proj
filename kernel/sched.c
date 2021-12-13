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

struct proc*
get()
{
    acquire(&schedlock);
    struct proc *ret = sjf_get();
    release(&schedlock);
    return ret;
}

void
put(struct proc *p)
{
    if (!p) panic("sched: put null");
    acquire(&schedlock);
    sjf_put(p);
    release(&schedlock);
}
