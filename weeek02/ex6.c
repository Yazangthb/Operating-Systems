#include <stdio.h>

// Function to print a right triangle pattern
void printRightTriangle(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

// Function to print a triangle with two equal sides whose base is on the left
void printLeftTriangle(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
    for (int i = n - 1; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

// Function to print a rectangle pattern
void printRectangle(int rows, int columns) {
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= columns; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter the number of rows for the right triangle and left triangle: ");
    scanf("%d", &n);

    printf("\nRight Triangle Pattern:\n");
    printRightTriangle(n);

    printf("\nTriangle with Two Equal Sides Pattern:\n");
    printLeftTriangle(n);

    int rows, columns;
    printf("\nEnter the number of rows and columns for the rectangle: ");
    scanf("%d %d", &rows, &columns);

    printf("\nRectangle Pattern:\n");
    printRectangle(rows, columns);

    return 0;
}
