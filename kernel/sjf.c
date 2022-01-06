//
// Created by aleksa on 12/11/21.
//

#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"


uint8 sjf_active = 1;

// Fixed point decimal number
// 100 == 1.0
// 50 == 0.5
uint64 sjf_alpha = 50;

void
sjf_set(uint8 status)
{
  sjf_active = status;
}

void
sjf_setalpha(uint64 alpha)
{
  sjf_alpha = alpha;
}


uint8
sjf_proc_lt(struct proc *a, struct proc *b)
{
    if (!sjf_active) panic("sjf inactive");
    if (a->tau < b->tau) return 1;
    return 0;
}

struct proc*
sjf_get()
{
    if (!sjf_active) panic("sjf inactive");
    return heap_pop();
}

void
sjf_put(struct proc *p)
{
    if (!sjf_active) panic("sjf inactive");
    if (!p) return;
    heap_push(p);
}

uint64
sjf_calc_tau(uint64 burst_len, uint64 old_tau)
{
    return (burst_len * sjf_alpha) / 100 + (old_tau * (100 - sjf_alpha)) / 100;
}
