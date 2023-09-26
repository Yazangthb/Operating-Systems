#include <stdio.h>
#include <math.h>

// Define a structure Point with fields x and y
struct Point {
    double x;
    double y;
};

// Function to calculate the Euclidean distance between two points
double distance(struct Point a, struct Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

// Function to calculate the area of a triangle formed by three points
double area(struct Point a, struct Point b, struct Point c) {
    return 0.5 * fabs(a.x * b.y - b.x * a.y + b.x * c.y - c.x * b.y + c.x * a.y - a.x * c.y);
}

int main() {
    // Define points A, B, and C
    struct Point A = {2.5, 6};
    struct Point B = {1, 2.2};
    struct Point C = {10, 6};

    // Calculate the distance between points A and B
    double distAB = distance(A, B);

    // Calculate the area of triangle ABC
    double triArea = area(A, B, C);

    // Print the results
    printf("Distance between A and B: %.2lf\n", distAB);
    printf("Area of triangle ABC: %.2lf\n", triArea);

    return 0;
}
