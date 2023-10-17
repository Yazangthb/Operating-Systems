#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#define PS_MAX 10

// ... (Rest of the code remains the same)

// Time quantum for the Round-Robin algorithm
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

// ... (Rest of the code remains the same)

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

    set_time_quantum();  // Set the time quantum

    // ... (Rest of the code remains the same)
}

