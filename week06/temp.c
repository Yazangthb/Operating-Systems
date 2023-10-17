#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#define PS_MAX 10

// holds the scheduling data of one process
typedef struct
{
    int idx;                     // process idx (index)
    int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
    int burst, arrival_time;     // remaining burst (this should decrement when the process is being executed)
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;
int time_quantum = 1;  // Default value

void set_time_quantum() {
    // Prompt the user to enter the time quantum from stdin
    printf("Enter the time quantum: ");
    scanf("%d", &time_quantum);
    if (time_quantum <= 0) {
        printf("Invalid time quantum. Using default value (1 second).\n");
        time_quantum = 1;
    }
    printf("Using time quantum: %d second(s).\n", time_quantum);
}


void read_file(FILE *file)
{
    char line[1000];
    data_size = 0;

    // Read data from the file until the end is reached or the maximum PS_MAX processes are read.
    while (data_size < PS_MAX && fgets(line, sizeof(line), file) != NULL)
    {
        int at, bt;
        if (sscanf(line, "%d %d %d", &data_size, &at, &bt) == 3)
        {
            // Initialize other fields in the data structure.
            data[data_size].idx = data_size;
            data[data_size].rt = data[data_size].bt;
            data[data_size].at = at;
            data[data_size].bt = bt;
            data[data_size].wt = 0;
            data[data_size].ct = 0;
            data[data_size].tat = 0;
            data[data_size].arrival_time = at;
            data[data_size].burst = bt; // Initialize remaining burst to 0 initially.
            // Increase data_size to indicate that we have read one process.
            data_size++;
        }
        }

    // Initialize the ps array to zeros (no processes created yet).
    for (int i = 0; i < PS_MAX; i++)
    {
        ps[i] = 0;
    }
}

void resume(pid_t process)
{
    // Check if the process is not created yet or terminated.
    if (process > 0)
    {
        // Send the SIGCONT signal to the process.
        if (kill(process, SIGCONT) == -1)
        {
            // Handle an error if necessary.
            perror("Error sending SIGCONT to the process");
        }
    }
    else
    {
        // Handle the case when the process is not created yet or has terminated.
        // You can print an error message or take other appropriate actions.
        // For example:
        fprintf(stderr, "Process is not created yet or has terminated.\n");
    }
}
void suspend(pid_t process)
{
    // Check if the process is not created yet or terminated.
    if (process > 0)
    {
        // Send the SIGTSTP signal to the process.
        if (kill(process, SIGTSTP) == -1)
        {
            // Handle an error if necessary.
            perror("Error sending SIGTSTP to the process");
        }
    }
    else
    {
        // Handle the case when the process is not created yet or has terminated.
        // You can print an error message or take other appropriate actions.
        // For example:
        fprintf(stderr, "Process is not created yet or has terminated.\n");
    }
}
void terminate(pid_t process)
{
    // Check if the process is not created yet or terminated.
    if (process > 0)
    {

        // Send the SIGTERM signal to the process.
        if (kill(process, SIGTERM) == -1)
        {
            // Handle an error if necessary.
            perror("Error sending SIGTERM to the process");
        }
    }
    else
    {
        // Handle the case when the process is not created yet or has terminated.
        // You can print an error message or take other appropriate actions.
        // For example:
        fprintf(stderr, "Process is not created yet or has terminated.\n");
    }
}


void create_process(int new_process) {
    if (running_process != -1) {
        suspend(running_process);
    }

    pid_t child_pid = fork();
    if (child_pid == 0) {
        char process_idx_str[10];
        sprintf(process_idx_str, "%d", new_process);
        char* args[] = {"./worker", process_idx_str, NULL};
        execvp(args[0], args);
    	//	exit(EXIT_SUCCESS);
    } else if (child_pid > 0) {
        ps[new_process] = child_pid;
        running_process = new_process;
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", new_process, data[new_process].burst);
    }
}

ProcessData find_next_process() {
    int location = 0;
    for (int i = 0; i < data_size; i++) {
        if (data[i].at < data[location].at) {
            location = i;
        }
    }

    if (data[location].at > total_time) {
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);
        total_time++;
        return find_next_process();
    }

    return data[location];
}

void report() {
    printf("Simulation results.....\n");
	int sum_wt = 0;
	int sum_tat = 0;
	for (int i=0; i< data_size; i++){
		printf("process %d: \n", i);
		printf("	at=%d\n", data[i].arrival_time);
		printf("	bt=%d\n", data[i].bt);
		printf("	ct=%d\n", data[i].ct);
		printf("	wt=%d\n", data[i].wt);
		printf("	tat=%d\n", data[i].tat);
		printf("	rt=%d\n", data[i].rt);
		sum_wt += data[i].wt;
		sum_tat += data[i].tat;
	}

	printf("data size = %d\n", data_size);
	float avg_wt = (float)sum_wt/data_size;
	float avg_tat = (float)sum_tat/data_size;
	printf("Average results for this run:\n");
	printf("	avg_wt=%f\n", avg_wt);
	printf("	avg_tat=%f\n", avg_tat);
}

void check_burst() {
    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0) {
            return;
        }
    }
    report();
    exit(EXIT_SUCCESS);
}


void schedule_handler(int signum)
{
    // Increment the total time.
    total_time++;

    // 1. If there is a worker process running, decrement its remaining burst by the time quantum.
    if (running_process >= 0)
    {
        data[running_process].burst -= time_quantum;

        // Print messages.
        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        // 1.A. If the worker process has finished its burst time, terminate it.
        if (data[running_process].burst <= 0)
        {
            printf("Scheduler: Terminating Process %d (Remaining Time: 0)\n", running_process);
            // Send SIGTERM to the running process.
            terminate(ps[running_process]);

            // Wait for the termination of the process.
            waitpid(ps[running_process], NULL, 0);

            // Calculate metrics (ct, tat, wt) for the terminated process.
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;

            data[running_process].at = 10000000; // [optional]
            data[running_process].burst = 0;

            // Reset the running process to -1 (no running process).
            running_process = -1;
        }
    }

    // 2. Find the next process to run (next_process).
    ProcessData next_process = find_next_process();

    // 3. If next_process is not running:
    if (!(next_process.idx == running_process))
    {
        running_process = next_process.idx;

        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);

        // Create a new process for running_process.
        create_process(running_process);

        // Calculate the response time (rt) for the newly started process.
        data[running_process].rt = total_time - data[running_process].at;
    }
}

int main(int argc, char *argv[]) {
    // ... (code before reading the file)
 set_time_quantum();
    // Read the data file
    FILE *in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);
    }


    // Set a timer
    struct itimerval timer;
timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // Register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    while (1); // Infinite loop
}

