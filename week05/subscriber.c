#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_MESSAGE_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "No arg provided!\n");
        exit(EXIT_FAILURE);
    }

    int subscriber_id = atoi(argv[1]);
    char pipe_name[20];

    sprintf(pipe_name, "/tmp/ex1/s%d", subscriber_id);
    int fd = open(pipe_name, O_RDONLY);

    if(fd == -1) {
        perror("open");
        printf("%s\n", pipe_name);
        return 1;
    }

    char message[MAX_MESSAGE_SIZE];
    ssize_t input;

    printf("subscriber:\n");
    while ((input = read(fd, message, MAX_MESSAGE_SIZE)) > 0) {
        write(STDOUT_FILENO, message, input);
    }

    close(fd);

    return 0;
}
