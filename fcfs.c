#include "sched.h"

static void computeFCFS(Process *procs, int n){
    //sort by arrival time
    for (int i = 1; i < n; i++){
    Process key = procs[i];
    int j = i-1;
    while (j>=0 && procs[j].arrival > key.arrival){
    procs[j+1] = procs[j];
    j--;
        }
    procs[j+1] = key;
    }
int t = 0;
for (int i = 0; i<n;i++){
    if (t<procs[i].arrival) t = procs[i].arrival;
    procs[i].start = t;
    procs[i].finish = t + procs[i].burst;
    procs[i].turnaround = procs[i].finish - procs[i].arrival;
    procs[i].waiting = procs[i].turnaround - procs[i].burst;
    t = procs[i].finish;
    }

}

int main (void){
RunScheduler("FCFS", "FCFS", computeFCFS);
return 0;
}
