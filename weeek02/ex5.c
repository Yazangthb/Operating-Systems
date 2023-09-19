#include <stdio.h>

// Function to calculate the Tribonacci number for a given n
int tribonacci(int n) {
    if (n == 0) return 0;
    if (n <= 2) return 1;

    int t0 = 0, t1 = 1, t2 = 1, tn = 0;

    for (int i = 3; i <= n; i++) {
        tn = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = tn;
    }

    return tn;
}

int main() {
    int n1 = 4;
    int n2 = 36;

    int result1 = tribonacci(n1);
    int result2 = tribonacci(n2);

    printf("Tribonacci(%d) = %d\n", n1, result1);
    printf("Tribonacci(%d) = %d\n", n2, result2);

    return 0;
}
