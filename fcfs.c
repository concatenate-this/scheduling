#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <string.h>

typedef struct{
    int pid, arrival, wait, burst, turnaround,start,finish;
}Process;

int compareArrival(const void *a, const void *b){
    return ((((Process*)a) -> arrival) - (((Process*)b) ->arrival) );
}

void fcfs (Process *proc, int n){
    //sort by arrival time

}
