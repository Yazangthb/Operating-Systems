#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define VECTOR_SIZE 120

// Function to calculate the dot product of two vectors
int dot_product(int u[], int v[], int start, int end) {
    int result = 0;
    for (int i = start; i < end; i++) {
        result += u[i] * v[i];
    }
    return result;
}

int main() {
    int u[VECTOR_SIZE], v[VECTOR_SIZE];
    int n;


    printf("Enter n which is the number of processes: ");
    scanf("%d", &n);

    srand(time(NULL));
    for (int i = 0; i < VECTOR_SIZE; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    FILE *file = fopen("temp.txt", "w+");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Create child processes
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            int start = i * (VECTOR_SIZE / n);
            int end = (i + 1) * (VECTOR_SIZE / n);
            int partial_result = dot_product(u, v, start, end);

            // Write the partial result to the file
            fprintf(file, "%d\n", partial_result);

            fclose(file);
            exit(EXIT_SUCCESS);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    // Calculate the final dot product by reading and aggregating the results from the file
    rewind(file);
    int final_result = 0;
    int partial_result;
    while (fscanf(file, "%d", &partial_result) != EOF) {
        final_result += partial_result;
    }

    // Close the file
    fclose(file);
    // Print the final result in the main process
    printf("The answer to the dot product of u  and v: %d\n", final_result);

    return 0;
}
