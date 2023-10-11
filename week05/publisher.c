#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_MESSAGE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_subscribers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    int n = atoi(argv[1]);
    if(n>3 || n<1){
    printf("number of subscribers is out of limit");
    exit(0);
    }
    int pipefd[n];
    char pipe_name[50];

    for (int i = 1; i <= n; i++) {
        sprintf(pipe_name, "/tmp/ex1/s%d", i);
        mkfifo(pipe_name, 0666);
    }

    for (int i = 1; i <= n; i++) {
        sprintf(pipe_name, "/tmp/ex1/s%d", i);
        pipefd[i-1] = open(pipe_name, O_WRONLY);
    }
    

    char message[MAX_MESSAGE];
    ssize_t input;

    while ((input = read(STDIN_FILENO, message, MAX_MESSAGE)) > 0) {

        for (int i = 1; i <= n; i++) {
            pid_t pid_i = fork();
            if (pid_i == 0) {
                sprintf(pipe_name, "/tmp/ex1/s%d", i);
                write(pipefd[i-1], message, input);
                exit(EXIT_SUCCESS);
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        close(pipefd[i-1]);
        sprintf(pipe_name, "/tmp/ex1/s%d", i);
        unlink(pipe_name);
    }

    return 0;
}
