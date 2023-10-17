#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void sigusr1_handler(int signum) {
    // Read and print the contents of text.txt
    char buffer[512];
    int fd = open("text.txt", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open text.txt");
    } else {
        ssize_t bytesRead;
        while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        close(fd);
    }
}

void sigusr2_handler(int signum) {
    // Handle SIGUSR2 - Terminate and print message
    printf("Process terminating...\n");
    exit(0);
}

int main() {
    // Create a PID file
    int pidfile = open("/var/run/agent.pid", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (pidfile == -1) {
        perror("Failed to create PID file");
        exit(1);
    }

    // Write the PID to the file
    pid_t pid = getpid();
    dprintf(pidfile, "%d\n", pid);
    close(pidfile);

    // Register signal handlers
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    // Print the contents of text.txt
    sigusr1_handler(SIGUSR1);

    // Sleep indefinitely
    while (1) {
        sleep(1);
    }

    return 0;
}

