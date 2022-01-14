//
// Created by aleksa on 12/14/21.
//

#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

uint32 heapsize = 0;
struct proc *heap[NPROC];
uint8(*comp)(struct proc*, struct proc*) = cfs_proc_lt;

void
min_heapify(uint32 i)
{
    uint32 l = 2 * i;
    uint32 r = l + 1;
    uint32 min = i;
    if (l < heapsize && comp(heap[l], heap[i]))
        min = l;
    if (r < heapsize && comp(heap[r], heap[min]))
        min = r;
    if (min != i){
        struct proc *t = heap[min];
        heap[min] = heap[i];
        heap[i] = t;
        min_heapify(min);
    }
}

void
heap_push(struct proc* p)
{
    // Inserting into the heap
    heapsize++;
    heap[heapsize - 1] = p;

    uint32 i = heapsize - 1;
    while (i > 0 && comp(heap[i], heap[i/2])) {
        struct proc *t = heap[i];
        heap[i] = heap[i / 2];
        heap[i / 2] = t;
        i /= 2;
    }
}

struct proc*
heap_pop()
{
    if (heapsize < 1) return 0;
    struct proc *ret = heap[0];
    heap[0] = heap[heapsize - 1];
    heapsize--;
    min_heapify(0);
    return ret;
}

void
heap_change_comp(uint8(*comparison)(struct proc*, struct proc*))
{
    comp = comparison;
    if (heapsize < 2) return;
    for (uint64 i = heapsize / 2; i >= 0; i--)
      min_heapify(i);
}