#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h> 
void print_process_info(const char* name) {
    printf("%s: Process ID=%d, Parent ID=%d\n", name, getpid(), getppid());
}

int main() {
    clock_t start_time, end_time;
    
    

    pid_t pid1 = fork();
    start_time = clock();

    if (pid1 == -1) {
        perror("Fork 1 failed");
        exit(1);
    } else if (pid1 == 0) {
        // This is the first child process
        
        print_process_info("Child 1");
        end_time = clock();
        double execution_time = ((double)(end_time - start_time) / CLOCKS_PER_SEC) *1000;
        printf("The child process nimber 1 execution time is =%.6f ms\n", execution_time);

        exit(EXIT_SUCCESS);
    }

    // Parent process
    pid_t pid2 = fork();
    start_time = clock();
    if (pid2 == -1) {
        perror("Fork 2 failed");
        exit(1);
    } else if (pid2 == 0) {
        // This is the second child process
        print_process_info("Child 2");
        end_time = clock();
        double execution_time = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000;
        printf("The child process nimber 1 execution time is =%.6f ms\n", execution_time);
        
        exit(0);
    }
  
    // Parent process
    end_time = clock();
    double execution_time = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000;
    
    print_process_info("Main");
    printf("The Main process execution time is =%.6f ms\n", execution_time);

 
    wait(NULL);
    wait(NULL);


    return 0;
}
