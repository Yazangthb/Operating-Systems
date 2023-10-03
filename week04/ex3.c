#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
int n = argc;
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork failed");
            exit(1);
        }

        else {
            // Child  or parent process
            printf("Child process %d created\n", getpid());
            sleep(5); // Sleep for 5 seconds
            exit(0);
        }
    }

    return 0;
}

