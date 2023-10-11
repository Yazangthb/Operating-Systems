
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int pipe_fd[2]; // File descriptors for the pipe

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        // Child process (Subscriber)
        close(pipe_fd[1]); // Close the write end of the pipe in the child process

        char message[1024];
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_fd[0], message, sizeof(message))) > 0)
        {
            // Print the received message to stdout
            write(STDOUT_FILENO, message, bytes_read);
        }

        close(pipe_fd[0]); // Close the read end of the pipe in the child process
    }
    else
    {
        // Parent process (Publisher)
        close(pipe_fd[0]); // Close the read end of the pipe in the parent process

        char message[1024];

        while (1)
        {
            // Read a message from stdin
            ssize_t bytes_read = read(STDIN_FILENO, message, sizeof(message));

            if (bytes_read <= 0)
            {
                break; // End of input or an error occurred
            }

            // Send the message to the child process through the pipe
            write(pipe_fd[1], message, bytes_read);
        }

        close(pipe_fd[1]); // Close the write end of the pipe in the parent process
    }

    return 0;
}
