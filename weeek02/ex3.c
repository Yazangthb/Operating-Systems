#include <stdio.h>
#include <math.h>

// Function to check if a number is valid in a given base
int isValidNumber(int x, int base) {
    while (x > 0) {
        int digit = x % 10;
        if (digit >= base) {
            return 0; // Invalid digit for the given base
        }
        x /= 10;
    }
    return 1; // Valid number
}

// Function to convert a number from base s to base t
void convert(int x, int s, int t) {
    if (s < 2 || s > 10 || t < 2 || t > 10) {
        printf("cannot convert!\n");
        return;
    }

    if (!isValidNumber(x, s)) {
        printf("cannot convert!\n");
        return;
    }

    // Convert from base s to base 10
    int decimal = 0;
    int position = 0;
    while (x > 0) {
        int digit = x % 10;
        decimal += digit * pow(s, position);
        x /= 10;
        position++;
    }

    // Convert from base 10 to base t
    int result[32]; // Assuming a maximum of 32 digits for the result
    int length = 0;
    while (decimal > 0) {
        result[length++] = decimal % t;
        decimal /= t;
    }

    // Print the result in reverse order
    printf("Converted number: ");
    for (int i = length - 1; i >= 0; i--) {
        printf("%d", result[i]);
    }
    printf("\n");
}

int main() {
    char input[100]; // Using a character array for input

    printf("Enter a non-negative number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    long long number;
    if (sscanf(input, "%lld", &number) != 1 || number < 0) {
        printf("Invalid input!\n");
        return 1;
    }

    printf("Enter source base (2-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    int source_base;
    if (sscanf(input, "%d", &source_base) != 1 || source_base < 2 || source_base > 10) {
        printf("Invalid input!\n");
        return 1;
    }

    printf("Enter target base (2-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    int target_base;
    if (sscanf(input, "%d", &target_base) != 1 || target_base < 2 || target_base > 10) {
        printf("Invalid input!\n");
        return 1;
    }

    convert((int)number, source_base, target_base);

    return 0;
}
