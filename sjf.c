//shortest job first- non preepmtive scheduling
//sort the queue from lowest to highest based on burst time and run the shortest one first
//advantage over FCFS- the shorter tasks executed first and they don't have to wait for a long time for the CPU, gives minimum average waiting time and less turnaround time as compared to FCFS
//Disadvantage- it is non-preemptive, if shorter jobs arrive not at t=0, if a long long job is running, the shorter jobs have to wait for the long long job to finish before being executed.

// Compile: gcc -O2 sjf.c sched_ui.c -o sjf -lraylib -lm
#include "sched.h"

static void computeSJF(Process *procs, int n) {
    int completed[MAX_PROCESSES] = {0};
    Process order[MAX_PROCESSES]; //reorder coz apparently that's good to do
    int t = 0, done = 0;

    while (done < n) {
        //pick via shortest burst time. if tie, then sort by arrival time.
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (completed[i] || procs[i].arrival > t) continue;
            if (idx == -1 || procs[i].burst < procs[idx].burst ||(procs[i].burst == procs[idx].burst && procs[i].arrival < procs[idx].arrival)) idx = i;
        }

        if (idx == -1) {
            //if nothing ready jumpppppp
            int next = INT_MAX;
            for (int i = 0; i < n; i++)
                if (!completed[i] && procs[i].arrival < next) next = procs[i].arrival;
            t = next;
            continue;
        }

        procs[idx].start = t;
        procs[idx].finish = t + procs[idx].burst;
        procs[idx].turnaround = procs[idx].finish - procs[idx].arrival;
        procs[idx].waiting = procs[idx].turnaround - procs[idx].burst;
        completed[idx] = 1;
        order[done++] = procs[idx];
        t= procs[idx].finish;
    }

    for (int i = 0; i < n; i++) procs[i] = order[i];
}

int main(void) {
    RunScheduler("SJF", "SJF", computeSJF);
    return 0;
}
