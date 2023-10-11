#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Define the Thread struct
struct Thread {
    int id;
    int i;
    char message[256];
};

// Mutex for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function for the thread to execute
void *thread_function(void *arg) {
    struct Thread *thread_data = (struct Thread *)arg;

    // Lock the mutex to ensure sequential execution
    pthread_mutex_lock(&mutex);

    // Print "Thread i is created"
    printf("Thread %d is created\n", thread_data->id);

    // Print the message
    printf("%s\n", thread_data->message);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    // Exit the thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int n; // Number of threads
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    } else {
        n = atoi(argv[1]);
    }

    // Create an array of n threads
    pthread_t threads[n];
    struct Thread thread_data[n];

    for (int i = 0; i < n; i++) {
        thread_data[i].id = i + 1;
        thread_data[i].i = i;
        snprintf(thread_data[i].message, sizeof(thread_data[i].message), "Hello from thread %d", i + 1);

        // Create the thread
        if (pthread_create(&threads[i], NULL, thread_function, &thread_data[i]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

