#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

bool is_prime(int n) {
    if (n <= 1)
        return false;
    
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    
    return true;
}

typedef struct prime_request {
    int a, b;
    int *result;
} prime_request;

void *prime_counter(void *arg) {
    prime_request *req = (prime_request *)arg;
    int count = 0;
    
    for (int i = req->a; i < req->b; i++) {
        if (is_prime(i)) {
            count++;
        }
    }
    
    *(req->result) = count;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    
    pthread_t threads[m];
    prime_request requests[m];
    int results[m];
    
    int interval_size = n / m;
    
    for (int i = 0; i < m; i++) {
        requests[i].a = i * interval_size;
        requests[i].b = (i == m - 1) ? n : (i + 1) * interval_size;
        requests[i].result = &results[i];
        
        pthread_create(&threads[i], NULL, prime_counter, &requests[i]);
    }
    
    int total_primes = 0;
    
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
        total_primes += results[i];
    }
    
    printf("Number of prime numbers in the range [0, %d) is: %d\n", n, total_primes);
    
    return 0;
}
