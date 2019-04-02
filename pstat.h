#ifndef _PSTAT_H
#define _PSTAT_H

#include "param.h"

struct pstat {
	int num_processes; // the total number of non-UNUSED processes in the process table
	int pid[NPROC]; // the PID of each process
	int tickets[NPROC]; // the number of tickets this process has
	int ticks[NPROC]; // the number of ticks each process has accumulated
};

#endif // _PSTAT_H_
