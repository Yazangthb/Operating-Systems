#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        // Prompt the user for input
        printf("MyShell> ");
        fgets(input, sizeof(input), stdin);

        // Remove the newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';

        // Check if the user entered "exit" to exit the shell
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Tokenize the input into command and arguments
        char* args[MAX_ARGS];
        int num_args = 0;

        char* token = strtok(input, " ");
        while (token != NULL) {
            args[num_args] = token;
            num_args++;

            // Ensure we don't exceed the maximum number of arguments
            if (num_args >= MAX_ARGS) {
                fprintf(stderr, "Too many arguments. Max is %d.\n", MAX_ARGS - 1);
                break;
            }

            token = strtok(NULL, " ");
        }

        args[num_args] = NULL; // Null-terminate the argument list

        // Check if the command should run in the background (ends with '&')
        int background = 0;
        if (num_args > 0 && strcmp(args[num_args - 1], "&") == 0) {
            background = 1;
            args[num_args - 1] = NULL; // Remove the '&' from arguments
        }

        // Fork a new process
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            // Execute the command using execve()
            if (execvp(args[0], args) == -1) {
                perror("Execution failed");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            if (!background) {
                // Wait for the child process to complete unless it's a background process
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    return 0;
}
