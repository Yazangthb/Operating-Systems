#include <stdio.h>
#include <stdlib.h>

// Function to calculate the Tribonacci number for n using constant pointer p
const int tribonacci(int* const p, int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    int temp = 0;
    for (int i = 3; i <= n; i++) {
        temp = p[0] + p[1] + p[2];
        p[0] = p[1];
        p[1] = p[2];
        p[2] = temp;
    }

    return p[2];
}

int main() {
    // Declare a constant integer x with a value of 1
    const int x = 1;

    // Declare a constant pointer q to x
    const int* q = &x;

    // Allocate three contiguous memory cells of type integer
    int *const p = (int *)malloc(3 * sizeof(int));
    if (p == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Using the pointer p, fill the first two cells with the value of x and the third cell with the value of 2x
    p[0] = *q;
    p[1] = *q;
    p[2] = 2 * (*q);

    // Print the memory addresses of these cells
    printf("Memory addresses:\n");
    for (int i = 0; i < 3; i++) {
        printf("address of p[%d]: %p\n", i, (void *)&p[i]);
    }

    // Check if the cells are contiguous
    if (&p[1] == (&p[0] + 1) && &p[2] == (&p[1] + 1)) {
        printf("The cells are contiguous.\n");
    } else {
        printf("The cells are not contiguous.\n");
    }

    // Calculate and print the Tribonacci number for n=6
    int n = 6;
    int result = tribonacci(p, n);
    printf("Tribonacci(%d) = %d\n", n, result);

    // Free the allocated memory
    free(p);

    return 0;
}
