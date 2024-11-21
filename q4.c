#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int min_index_fifo(int arr[], int n, int check[]) {
    int mini = -1;
    int minValue = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (!check[i] && arr[i] < minValue) {
            minValue = arr[i];
            mini = i;
        }
    }
    return mini;
}

int min_index_sjf(int arr[], int burst[], int n, int check[], int current_time) {
    int mini = -1;
    int minBurst = INT_MAX;

    for (int i=0;i<n;i++) {
        if (!check[i]&&arr[i]<=current_time) {
            if (burst[i]<minBurst) {
                minBurst=burst[i];
                mini=i;
            }
        }
    }
    return mini;
}

void rt_tt(int n,int arrival[],int burst[],int execution[]){
    float turnaround=0;
    float response=0;
    int total_arr=0;

    int temp[n];   
    temp[0]=burst[execution[0]-1];
    for (int i=1;i<n;i++) {
        temp[i]=temp[i-1]+burst[execution[i]-1];
    }

    for(int i=0;i<n;i++){
        turnaround+=temp[i];
        total_arr+=arrival[i];
    }

    turnaround-=total_arr;
    response=turnaround-temp[n-1];
    turnaround=turnaround/n;
    response=response/n;

    printf("Average Response Time: %.2f \n",response);
    printf("Average Turnaround Time: %.2f \n",turnaround);
    printf("\n");
}  

void rr_tt_2(int n, int arrival[], int start_time[], int end_time[]) {
    float turnaround = 0;
    float response = 0;
    int total_arr = 0;

    for (int i = 0; i < n; i++) {
        turnaround += end_time[i] - arrival[i];
        response += start_time[i] - arrival[i];
        total_arr += arrival[i];
    }

    turnaround /= n;
    response /= n;

    printf("Average Response Time: %.2f \n", response);
    printf("Average Turnaround Time: %.2f \n", turnaround);
    printf("\n");
}

void FIFO(int n,int pid[],int arrival[],int burst[]){
    printf("\n");
    printf("FIFO Scheduling Algorithm => \n");
    int execution[n];
    int check[n];

    for (int i = 0; i < n; i++) {
        check[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int a = min_index_fifo(arrival, n, check);
        if (a != -1) {
            execution[i] = pid[a];
            check[a] = 1;
        }
    }

    printf("Execution order: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", execution[i]);
    }
    printf("\n");

    rt_tt(n,arrival,burst,execution);
}

void SJF(int n, int pid[], int arrival[], int burst[]) {
    printf("SJF Scheduling Algorithm => \n");
    int execution[n];
    int check[n];
    int current_time = 0;

    for (int i=0;i<n;i++) {
        check[i]=0;
    }

    for (int i=0;i<n;i++) {
        int a = min_index_sjf(arrival, burst, n, check, current_time);
        if (a != -1) {
            execution[i] = pid[a];
            check[a] = 1;

            current_time += burst[a];
        }
    }

    printf("Execution order: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", execution[i]);
    }
    printf("\n");

    rt_tt(n,arrival,burst,execution);

    
}

void SRTF(int n, int pid[], int arrival[], int burst[]) {
    printf("SRTF Scheduling Algorithm => \n");

    int remaining_burst[n];
    int check[n];
    int start_time[n];
    int end_time[n];
    int current_time=0;
    int completed=0;

    for (int i=0;i<n;i++) {
        remaining_burst[i]=burst[i];
        check[i]=0;
        start_time[i]=-1;
        end_time[i]=-1;
    }

    printf("Execution order: ");
    
    while (completed<n) {
        int min_index=-1;
        int min_burst=INT_MAX;

        for (int i=0;i<n;i++) {
            if (arrival[i]<=current_time && !check[i] && remaining_burst[i]<min_burst) {
                min_burst=remaining_burst[i];
                min_index=i;
            }
        }

        if (min_index!=-1) {
            printf("%d ",pid[min_index]);

            if (start_time[min_index]==-1) {
                start_time[min_index]=current_time;
            }

            remaining_burst[min_index]--;
            current_time++;

            if (remaining_burst[min_index]==0) {
                check[min_index]=1;
                end_time[min_index]=current_time;
                completed++;
            }
        } else {
            current_time++;
        }
    }
    printf("\n");

    rr_tt_2(n,arrival,start_time,end_time);
    
}

void RoundRobin(int n, int pid[], int arrival[], int burst[], int quantum) {
    printf("Round Robin Scheduling Algorithm => \n");

    int remaining_burst[n];
    int check[n];
    int start_time[n];
    int end_time[n];
    int current_time = 0;
    int completed = 0;

    for (int i = 0; i < n; i++) {
        remaining_burst[i] = burst[i];
        check[i] = 0;
        start_time[i] = -1;
        end_time[i] = -1;
    }

    while (completed<n) {
        int all_done=1; 
        for (int i=0;i<n;i++) {
            if (arrival[i]<=current_time && !check[i]) {
                all_done=0; 
                if (start_time[i]==-1) {
                    start_time[i] = current_time;
                }

                int time_slice = (remaining_burst[i] < quantum) ? remaining_burst[i] : quantum;
                remaining_burst[i] -= time_slice;
                current_time += time_slice;

                printf("Execution order: ");
                printf("%d ", pid[i]);

                if (remaining_burst[i] == 0) {
                    check[i] = 1;
                    end_time[i] = current_time;
                    completed++;
                }
            }
        }
    }
    printf("\n");
    rr_tt_2(n,arrival,start_time,end_time);
}

int main() {
    int n;
    printf("Number of Processes:");
    scanf("%d", &n);

    if(n<=4){
        printf("ERROR:Processes should be more than 4");
        return 0;
    }

    int pid[n];
    int arrival[n];
    int burst[n];

    printf("Enter details for each process (PID, Arrival, Burst): \n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:", i + 1);
        scanf("%d %d %d", &pid[i], &arrival[i], &burst[i]);
        if(pid[i]<0 || arrival[i]<0 || burst[i]<=0){
            printf("ERROR: Wrong argument");
            return 0;}
    }

    int q;
    printf("Enter Time Quantum:");
    scanf("%d", &q);

    if(q<=0){
        printf("ERROR: quantum should be more than 0");
        return 0;
    }

    FIFO(n,pid,arrival,burst);
    SJF(n,pid,arrival,burst);
    SRTF(n,pid,arrival,burst);
    RoundRobin(n,pid,arrival,burst,q);

    
}
