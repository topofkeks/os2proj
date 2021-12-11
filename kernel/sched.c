//
// Created by aleksa on 12/11/21.
//
#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "sjf.h"
#include "cfs.h"
#include "sched.h"

struct proc*
get()
{
    return sjf_get();
}

void
put(struct proc *p)
{
    sjf_put(p);
}

void
init(void)
{
    sjf_init();
}