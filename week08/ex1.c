#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

void writeProcessID() {
    FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    if (pid_file == NULL) {
        perror("Error opening /tmp/ex1.pid");
        exit(1);
    }
    fprintf(pid_file, "%d\n", getpid());
    fclose(pid_file);
}

void generatePassword() {
    char password[9];
    int urandom_fd = open("/dev/urandom", O_RDONLY);
    if (urandom_fd == -1) {
        perror("Error opening /dev/urandom");
        exit(1);
    }
    strcpy(password, "pass:");
    for (int i = 5; i < 8; i++) {
        unsigned char random_char;
        if (read(urandom_fd, &random_char, 1) == -1) {
            perror("Error reading from urandom");
            exit(1);
        }
        password[i] = 33 + (random_char % 94);
    }
    password[8] = '\0';

    char *password_mmap = (char *)mmap(NULL, sizeof(password), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (password_mmap == MAP_FAILED) {
        perror("Error in mmap");
        exit(1);
    }
    strcpy(password_mmap, password);
    printf("Password is: %s\n", password_mmap);
}

int main() {
    writeProcessID();
    generatePassword();

    while (1) {
        sleep(1);
    }

    return 0;
}

