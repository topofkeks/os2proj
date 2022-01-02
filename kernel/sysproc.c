#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sched(void)
{
  int sched, timeslice, alpha;
  argint(0, &sched);
  argint(1, &timeslice);
  argint(2, &alpha);
  if (alpha > 100) alpha = 100;
  acquire(&schedlock);
  switch (sched){
    case 0:
      sjf_set(1);
      cfs_set(0);
      set_sched(sjf_get, sjf_put);
      sjf_setalpha(alpha);
      set_timeslice(timeslice);
      heap_change_comp(sjf_proc_lt);
      break;
    case 1:
      cfs_set(1);
      sjf_set(0);
      set_sched(cfs_get, cfs_put);
      heap_change_comp(cfs_proc_lt);
      break;
    default: panic("invalid scheduler");
  }
  release(&schedlock);
  return 0;
}