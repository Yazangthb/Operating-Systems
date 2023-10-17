#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Global variable to track if Ctrl+C was received
volatile sig_atomic_t ctrl_c_received = 0;

// Signal handler for Ctrl+C (SIGINT)
void sigint_handler(int signum) {
    ctrl_c_received = 1;
}

int main() {
    // Check if the agent's PID file exists
    FILE *pidfile = fopen("/var/run/agent.pid", "r");
    if (pidfile == NULL) {
        perror("Error: No agent found.");
        exit(1);
    }

    pid_t agent_pid;
   if (fscanf(pidfile, "%d", &agent_pid) != 1) {
        perror("Failed to read PID from file");
        fclose(pidfile);
        exit(1);
    }
    fclose(pidfile);

    printf("Agent found.\n");

    // Register the SIGINT (Ctrl+C) handler
    signal(SIGINT, sigint_handler);

    while (1) {
        if (ctrl_c_received) {
            // Send SIGTERM to the agent and then exit
            if (kill(agent_pid, SIGTERM) == -1) {
                perror("Failed to send SIGTERM");
            }
            break;
        }

        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"}: ");
        char command[20];
        if (scanf("%19s", command) != 1) {
            perror("Failed to read user input");
            break;
        }

        if (strcmp(command, "read") == 0) {
            // Send SIGUSR1 to the agent
            if (kill(agent_pid, SIGUSR1) == -1) {
                perror("Failed to send SIGUSR1");
            }
        } else if (strcmp(command, "exit") == 0) {
            // Send SIGUSR2 to the agent and then exit
            if (kill(agent_pid, SIGUSR2) == -1) {
                perror("Failed to send SIGUSR2");
            }
            break;
        } else if (strcmp(command, "stop") == 0) {
            // Send SIGSTOP to the agent
            if (kill(agent_pid, SIGSTOP) == -1) {
                perror("Failed to send SIGSTOP");
            }
        } else if (strcmp(command, "continue") == 0) {
            // Send SIGCONT to the agent
            if (kill(agent_pid, SIGCONT) == -1) {
                perror("Failed to send SIGCONT");
            }
        } else {
            printf("Invalid command. Try again.\n");
        }
    }

    return 0;
}

