#include "sched.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: Missing algorithm argument.\n");
        printf("Usage: %s [fcfs | sjf | stcf]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "fcfs") == 0) {
        RunScheduler("FCFS Scheduler", "FIRST COME FIRST SERVED", computeFCFS);
    } 
    else if (strcmp(argv[1], "sjf") == 0) {
        RunScheduler("SJF Scheduler", "SHORTEST JOB FIRST", computeSJF);
    } 
    else if (strcmp(argv[1], "stcf") == 0) {
        RunScheduler("STCF Scheduler", "SHORTEST TIME-TO-COMPLETION FIRST", computeSTCF);
    } 
    else {
        printf("Error: Unknown algorithm '%s'.\n", argv[1]);
        printf("Available options: fcfs, sjf, stcf\n");
        return 1;
    }

    return 0;
}