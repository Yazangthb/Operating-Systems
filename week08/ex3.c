#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    struct rusage usage;
    int seconds = 10;
    int sizeOfMemory = 1000* 1024 * 1024; 
char* mem_block ;
    for (int i = 0; i < seconds; i++) {

        mem_block = (char*)malloc(sizeOfMemory);

        if (mem_block == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }

      memset(mem_block, 0, sizeOfMemory);

        if (getrusage(RUSAGE_SELF, &usage) == 0) {
            printf("Memory Usage in MB Second number %d:\n", i + 1);
            printf("Maximum Resident Set Size: %ld\n", usage.ru_maxrss);
            printf("Integral Shared Memory Size is: %ld\n", usage.ru_ixrss);
            printf("Integral Unshared Data Size is: %ld\n", usage.ru_idrss);
            printf("Integral Unshared Stack Size is: %ld\n", usage.ru_isrss);
        } else {
            perror("fail");
        }

        sleep(1);
    }
     
    return 0;
}
