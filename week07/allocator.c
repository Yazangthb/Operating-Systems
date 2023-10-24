#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEM_SIZE 10000000
#define ALLOCATIONS_FILE "queries.txt"

unsigned int memory[MEM_SIZE];

void initializeMemory() {
    memset(memory, 0, sizeof(memory));
}

int firstFit(unsigned int size) {
    for (int i = 0; i < MEM_SIZE; i++) {
        int block_size = 0;
        while (i < MEM_SIZE && memory[i] == 0) {
            block_size++;
            if (block_size >= size) {
                for (int j = i - block_size + 1; j <= i; j++) {
                    memory[j] = size;
                }
                return i - block_size + 1;
            }
            i++;
        }
    }
    return -1; // No block found
}

int bestFit(unsigned int size) {
    int best_fit = -1;
    int best_fit_size = MEM_SIZE;

    for (int i = 0; i < MEM_SIZE; i++) {
        int block_size = 0;
        while (i < MEM_SIZE && memory[i] == 0) {
            block_size++;
            if (block_size >= size) {
                if (block_size < best_fit_size) {
                    best_fit = i - block_size + 1;
                    best_fit_size = block_size;
                }
                i++;
            }
            else {
                i++;
            }
        }
    }

    if (best_fit != -1) {
        for (int j = best_fit; j < best_fit + size; j++) {
            memory[j] = size;
        }
    }

    return best_fit;
}

int worstFit(unsigned int size) {
    int worst_fit = -1;
    int worst_fit_size = -1;

    for (int i = 0; i < MEM_SIZE; i++) {
        int block_size = 0;
        while (i < MEM_SIZE && memory[i] == 0) {
            block_size++;
            if (block_size >= size) {
                if (block_size > worst_fit_size) {
                    worst_fit = i - block_size + 1;
                    worst_fit_size = block_size;
                }
                i++;
            }
            else {
                i++;
            }
        }
    }

    if (worst_fit != -1) {
        for (int j = worst_fit; j < worst_fit + size; j++) {
            memory[j] = size;
        }
    }

    return worst_fit;
}

void clear(unsigned int size) {
    for (int i = 0; i < MEM_SIZE; i++) {
        if (memory[i] == size) {
            memory[i] = 0;
        }
    }
}

int main() {
    initializeMemory();
    FILE *file = fopen(ALLOCATIONS_FILE, "r");
    clock_t start_time, end_time;
    char operation[20];
    unsigned int size;
    int allocations, deallocations;

    if (file == NULL) {
        printf("Error: Could not open %s\n", ALLOCATIONS_FILE);
        return 1;
    }

    FILE *output = fopen("ex1.txt", "w");
    
    int (*allocation_functions[])(unsigned int) = {firstFit, bestFit, worstFit};
    char *algorithm_names[] = {"First Fit", "Best Fit", "Worst Fit"};

    for (int algorithm = 0; algorithm < 3; algorithm++) {
        initializeMemory();
        start_time = clock();
        allocations = 0;
        deallocations = 0;

        while (fscanf(file, "%s", operation) != EOF) {
            if (strcmp(operation, "allocate") == 0) {
                fscanf(file, "%u", &size);
                int start_index = allocation_functions[algorithm](size);

                if (start_index != -1) {
                    allocations++;
                }
            } else if (strcmp(operation, "clear") == 0) {
                fscanf(file, "%u", &size);
                clear(size);
                deallocations++;
            }
        }

        end_time = clock();

        double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        double throughput = (allocations + deallocations) / execution_time;

        fprintf(output, "Algorithm: %s\n", algorithm_names[algorithm]);
        fprintf(output, "Throughput: %.2lf\n", throughput);
        fprintf(output, "\n");

        // reset the file pointer to the beginning
        fseek(file, 0, SEEK_SET);
    }
    fprintf(output, "\nWe can see that the number of queries in the first fit is the highest then comes best fit and worst fit which are usually close to each other\n");
    fclose(file);
    fclose(output);

    return 0;
}


