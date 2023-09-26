#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // for INT_MIN constant
#include <float.h>
// Function pointer typedef for the aggregation operation

void* result;
// Function to aggregate elements in an array
void* aggregate(void* base, size_t size, int n, void* initialValue, void *(*opr)(const void *, const void *)) {
    void * answer = initialValue;

    for (int i = 0; i < n; i++) {
        answer = opr(answer, (base+i*size));
        }

    return answer;
}

// Aggregation operation for addition (works for int and double)
void* addition_int(const void* a, const void* b) {
        result = malloc(sizeof(int));
        *(int *)result = *(int*)a + *(int*)b;
        return result;
}

void* addition_double(const void* a, const void* b) {
        result = malloc(sizeof(double));
        *(double *)result = *(double*)a + *(double*)b;
        return result;
}
// Aggregation operation for multiplication (works for int and double)
void* multiplication_int(const void* a, const void* b) {

        result = malloc(sizeof(int));
        *(int *)result = (*(int*)a) *(*(int*)b);
        return result;

}
void* multiplication_double(const void* a, const void* b) {

        result = malloc(sizeof(double));
        *(double *)result = (*(double*)a) * (*(double*)b);
        return result;

}
// Aggregation operation for finding the max (works for int and double)
void* findMax_int(const void* a, const void* b) {

        return  (*(int*)a > *(int*)b) ? (int*)a : (int*)b;

}
void* findMax_double(const void* a, const void* b) {

        return  (*(double*)a > *(double*)b) ? (double*)a : (double*)b;

}

int main() {
    int intArray[5] = {1, 2, 3, 4, 5};
    double doubleArray[5] = {1.1, 2.2, 3.3, 4.4, 5.5};

    // Test addition aggregation on integers
    int initialIntAddition = 0;
    int* resultIntAddition = (int*)aggregate(intArray, sizeof(int), 5, &initialIntAddition, addition_int);
    printf("Sum of integers: %d\n", *resultIntAddition);

    // Test multiplication aggregation on doubles
    double initialDoubleMultiplication = 1.0;
    double* resultDoubleMultiplication = (double*)aggregate(doubleArray, sizeof(double), 5, &initialDoubleMultiplication, multiplication_double);
    printf("Product of doubles: %.2lf\n", *resultDoubleMultiplication);

    // Test max aggregation on integers
    int initialIntMax = INT_MIN; // Initialize with the minimum integer value
    int* resultIntMax = (int*)aggregate(intArray, sizeof(int), 5, &initialIntMax, findMax_int);
    printf("Max of integers: %d\n", *resultIntMax);

    // Test max aggregation on doubles
    double initialDoubleMax = -DBL_MAX; // Initialize with the minimum double value
    double* resultDoubleMax = (double*)aggregate(doubleArray, sizeof(double), 5, &initialDoubleMax, findMax_double);
    printf("Max of doubles: %.2lf\n", *resultDoubleMax);

    return 0;
}
