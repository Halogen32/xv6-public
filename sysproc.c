#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

// ADDED
#include "pstat.h"
#include "spinlock.h" // used by proctable.h
#include "proctable.h"

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
  // cprintf("entered ticket setter\n");
  
  //const int MAX_TICKETS = 100000;
  //const int MIN_TICKETS = 1;
  // const int DEFAULT_TICKETS = 10;
  
  // cprintf("init local variables\n");
  
  int tickets = 0;
  int code = argint(0, &tickets); // acquire ticket argument
  struct proc *p = myproc(); // get the current calling process
  
	// cprintf("tickets to set: %d\n", tickets);
  // cprintf("PASSED!\n");
  
	if (code < 0) // the method failed to acquire ticket argument
		return -1; // fail
		
	// cprintf("passed code check\n");
	
	if (tickets < MIN_TICKETS || tickets > MAX_TICKETS) // the method is requesting to set unreasonable tickets
	{
	  return -1; // fail
	}
	
	// cprintf("passed ticket range test\n");
	
	p->tickets = tickets; // sets the number of tickets this process has
	
  return 0; // success
  
}

// get information about all running processes
int
sys_getpinfo(void) {
  
  struct proctable *pt = getptable(); // gets ptable from proc.c
  struct pstat *ps; // container for all kinds of running process information defined in pstat.h
  struct proc *p; // a reference to a process
  
  int code = argptr(0, (void*)&ps, sizeof(struct pstat)); // acquire argument
  
  if (code < 0) return -1; // method failed to acquire argument
  if (ps == 0) return -1; // can't work with null values
  
  acquire(&pt->lock); // lock ptable
  ps->num_processes = 0;
  
  int i = 0; // index integer
  for (p = pt->proc; p < &pt->proc[NPROC]; p++) { // scan all created processes
    
    
    if (p->state != UNUSED) {
    	ps->num_processes++; // non-UNUSED processes increase
    
      // cprintf("process %d, queued %d\n", p->pid, ps->num_processes);
		  ps->pid[i] = p->pid; // assign pid within pid range
		  ps->tickets[i] = p->tickets; // capture tickets within range
		  ps->ticks[i] = p->ticks; // capture ticks used by process
		  
		  i = (i + 1) % NPROC; // next proc index
    }
    
  }
    
  release(&pt->lock); // unlock ptable
  
  return 0; // success  
  
}

// calls the real implementation of yield and return success
int
sys_yield(void) {

  yield(); // calls the real implementation from defs.h and defined in proc.c
  return 0; // always succeed
  
}
















