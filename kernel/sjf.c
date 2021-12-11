//
// Created by aleksa on 12/11/21.
//

#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "sjf.h"


uint8 sjf_active = 0;
uint8 sjf_preemptive = 0;
uint64 sjf_default_time_slice = 100;

// Fixed point decimal number
// 100 == 1.0
// 50 == 0.5
uint64 sjf_alpha = 50;

struct proc *sjf_heap[NPROC];
uint32 sjf_heapsize = 0;

uint8
sjf_proc_lt(struct proc *a, struct proc *b)
{
    if (a->prediction < b->prediction) return 1;
    return 0;
}

void
sjf_max_heapify(uint32 i)
{
    uint32 l = 2 * i;
    uint32 r = l + 1;
    uint32 min = i;
    if (l < sjf_heapsize && sjf_proc_lt(sjf_heap[l], sjf_heap[i]))
        min = l;
    if (r < sjf_heapsize && sjf_proc_lt(sjf_heap[r], sjf_heap[min]))
        min = r;
    if (min != i){
        struct proc *t = sjf_heap[min];
        sjf_heap[min] = sjf_heap[i];
        sjf_heap[i] = t;
        sjf_max_heapify(min);
    }
}

struct proc*
sjf_get()
{
    if (!sjf_active) return NULL;
    if (sjf_heapsize < 1) return NULL;
    struct proc *ret = sjf_heap[0];
    sjf_heap[0] = sjf_heap[sjf_heapsize - 1];
    sjf_max_heapify(0);
    return ret;
}

void
sjf_put(struct proc *p)
{
    if (!sjf_active) return;
    if (!p) return;

    // Recalculating prediction if blocked on I/O
    if (p->state == SLEEPING){
        p->prediction = p->burst_time * sjf_alpha / 100 + p->prediction * (100 - sjf_alpha) / 100;
        p->burst_time = 0;
        p->time_slice = sjf_default_time_slice;
    }

    // Inserting into the heap
    sjf_heapsize++;
    sjf_heap[sjf_heapsize - 1] = p;

    uint32 i = sjf_heapsize - 1;
    while (i > 0 && sjf_proc_lt(sjf_heap[i], sjf_heap[i/2])) {
        struct proc *t = sjf_heap[i];
        sjf_heap[i] = sjf_heap[i / 2];
        sjf_heap[i / 2] = t;
        i /= 2;
    }
}

void
sjf_init(void)
{
    sjf_heapsize = 0;
    for (uint32 p = 0; p < NPROC; p++)
        sjf_heap[p] = {NULL, 0, 0};
}