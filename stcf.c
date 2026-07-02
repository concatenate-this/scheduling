// stcf.c — Shortest Time-to-Completion First (Preemptive SJF)
#include "sched.h"
#include <limits.h>

void computeSTCF(Process *procs, int n) {
    int rem[MAX_PROCESSES];
    for (int i = 0; i < n; i++) {
        rem[i] = procs[i].burst;
        procs[i].start = -1; //-1 to indicate it hasn't started yet
    }

    int completed = 0, t = 0;
    
    while (completed < n) {
        int idx = -1;
        int min_rem = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival <= t && rem[i] > 0 && rem[i] < min_rem) {
                min_rem = rem[i];
                idx = i;
            }
        }

        if (idx != -1) {
            //record start, if starting for the first time
            if (procs[idx].start == -1) {
                procs[idx].start = t;
            }
            
            rem[idx]--;
            t++;
            
            if (rem[idx] == 0) {
                completed++;
                procs[idx].finish = t;
                procs[idx].turnaround = procs[idx].finish - procs[idx].arrival;
                procs[idx].waiting = procs[idx].turnaround - procs[idx].burst;
            }
        } else {
            //CPU is idle
            t++; 
        }
    }
    
    //Sort processes by start time to maintain some order in the non-preemptive UI
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (procs[j].start > procs[j+1].start) {
                Process temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }
}