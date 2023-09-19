#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    int integer_var;
    unsigned short unsigned_short_var;
    long int signed_long_int_var;
    float float_var;
    double double_var;

    // Assign maximum values for each variable
    integer_var = INT_MAX;
    unsigned_short_var = USHRT_MAX;
    signed_long_int_var = LONG_MAX;
    float_var = FLT_MAX;
    double_var = DBL_MAX;

    // Print sizes and values of each variable
    printf("Size of int: %lu bytes\n", sizeof(int));
    printf("Maximum value of int: %d\n", integer_var);

    printf("Size of unsigned short: %lu bytes\n", sizeof(unsigned short));
    printf("Maximum value of unsigned short: %u\n", unsigned_short_var);

    printf("Size of signed long int: %lu bytes\n", sizeof(long int));
    printf("Maximum value of signed long int: %ld\n", signed_long_int_var);

    printf("Size of float: %lu bytes\n", sizeof(float));
    printf("Maximum value of float: %e\n", float_var);

    printf("Size of double: %lu bytes\n", sizeof(double));
    printf("Maximum value of double: %e\n", double_var);

    return 0;
}
