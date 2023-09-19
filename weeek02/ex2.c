#include <stdio.h>
#include <string.h>

int main() {
    char input[257]; // Declare a character array to store the input string (maximum size 256)
    char reverse[257]; // Declare a character array to store the reversed string
    int length, i;

    printf("Enter a string character-by-character (maximum size 256):\n");

    // Read input character by character until '.' or Enter is pressed
    for (i = 0; i < 256; i++) {
        char c = getchar();

        if (c == '.' || c == '\n') {
            input[i] = '\0'; // Null-terminate the input string
            break;
        }

        input[i] = c;
    }

    length = strlen(input);

    // Reverse the input string
    for (i = 0; i < length; i++) {
        reverse[i] = input[length - i - 1];
    }
    reverse[length] = '\0'; // Null-terminate the reversed string

    printf("Reversed string with double quotation: \"%s\"\n", reverse);

    return 0;
}
