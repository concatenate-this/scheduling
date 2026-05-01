#include <stdio.h>

int main(){
    int arrival[10],burst[10],completion[10], turnaround[10], wait[10];
    int sum=0, n;
    int totalWait=0, totalTA=0;
    
     printf("Enter number of processes: ");
     scanf ("%d", &n);

     for (int i=0;i<n;i++){
         printf("\nEnter arrival time of process [%d]:", i+1);
         scanf("%d", &arrival[i]);

         printf("\nEnter burst time of process [%d]:", i+1);
         scanf("%d", &burst[i]);
     }

     //completion time
     for (int i=0;i<n;i++){
         sum +=burst[i];
         completion[i]+=sum;
     }

     //turnaround time
     for (int i=0;i<n;i++){
         turnaround[i] = completion[i]-arrival[i];
         totalTA = totalTA+ turnaround[i];
     }

     //wait time
     for (int i=0;i<n;i++){
         wait[i] = turnaround[i] - burst[i];
         totalWait =totalWait+ wait[i];
     }
     printf("\n\n\n---------------FCFS---------------\n");
     printf("Proc No.\t Arrival\t Burst\t Completion\t Turnaround \t Wait\n");
     for (int i=0;i<n;i++){
         printf("%d\t %d\t %d\t %d\t %d\t %d\t", i+1, arrival[i],burst[i],completion[i], turnaround[i],wait[i]);
         printf("\n");
     }

     printf("\n\nAverage turnaround time = %d", totalTA/n);
     printf("\nAverage wait time = %d", totalWait/n);

return 0;
}

