#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->vlimit;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// all of them have void and formal parameters appear to differ
int
sys_mprotect(void)
{
  void *addr;
  int len;
  struct proc *curproc = myproc();
  // FAILURE CASES
  // CASE 2: Addr points to a region not in address space
  // seen something in vm.c in argptr
  // Not sure how to call
  if(argint(0, &len) < 0) {
    return -1;
  }
  addr = (void *)myproc()->vlimit;
  if((int) addr >= curproc->vlimit ||(int) addr < curproc->vbase) return -1;

  // CASE 1: seen in vm.c in loaduvm:  addr must be page aligned);
  if((int)addr%PGSIZE != 0 ){
    return -1;
  }

  // CASE 3: Len is negative or 0
  if(len <= 0){
    return -1;
  }
  return mprotect(addr,len);
  //return 0;
}

// all of them have void and formal parameters appear to differ
int
sys_munprotect(void) {
  void *addr;
  int len;
  // FAILURE CASES
  // CASE 2: Addr points to a region not in address space
  // seen something in vm.c in argptr
  // Not sure how to call
  // initialize len like in this file sys_sleep
  if(argint(0, &len) < 0){
    return -1;
  }
  // initialize addr like in this file sys_sbrk
  addr = (void *)myproc()->vlimit;

  // CASE 1: seen in vm.c in loaduvm:  addr must be page aligned);
  if((int)addr%PGSIZE != 0 ){
    return -1;
  }
  // CASE 3: Len is negative or 0
  if(len <= 0){
    return -1;
  }
  return munprotect(addr,len);
  //return 0;

}
