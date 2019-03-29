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
  addr = myproc()->sz;
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

// set the number of tickets in the calling process
int
sys_settickets(void) {
  const int MAX_TICKETS = 100000;
  const int MIN_TICKETS = 1;
  const int DEFAULT_TICKETS = 10;
  
  int *number;
  int code = argint(0, number); // acquire argument
  
	if (code < 0) // the method failed to acquire desired tickets
	  return -1; // fail
	  
	if (*number < MIN_TICKETS || *number > MAX_TICKETS) // the method is requesting to set unreasonable tickets
	  return -1; // fail
	
	// TODO: SET TICKETS FOR THE CALLING PROCESS
	
  return 0; // success
}

// get information about all running processes
int
sys_getpinfo(void) {
  
  struct pstat *ps;
  struct proc *p;
  
  int code = argptr(0, (void*)&ps, sizeof(struct pstat)); // acquire argument
  
  if (code < 0) // failed to process argument
    return -1; // fail
  
  acquire(&ptable.lock); // lock ptable
  
  ps->num_processes = 0;
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) { // scan all created processes
    
    ps->num_processes++;
    ps->pid[p % NPROC] = p; // assign pid within pid range
    
    // TODO: FIGURE OUT HOW TO ASSIGN PROCESS ID
    // TODO: HOW TO RECORD TICKETS
    
  }
  release(&ptable.lock); // unlock ptable
  
  return 0; // success  
}

// calls the real implementation of yield and return success
int
sys_yield(void) {

  yield(); // calls the real implementation
  return 0; // always succeed
}
















