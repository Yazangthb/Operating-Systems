#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count(char str[], char target) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (tolower(str[i]) == tolower(target)) {
            count++;
        }
    }
    return count;
}

void countAll(char str[]) {
    int length = strlen(str);
    for (int i = 0; i < length; i++)
    {
        char current = str[i];
        current = tolower(current);
        int Count = count(str, current);
        printf("%c:%d, ", current, Count);
    }
    printf("\n");
}

int main() {
    char input[256];
    printf("Enter a string: ");
    scanf("%255s", input);


    printf("Character Counts in \"%s\": ", input);
    countAll(input);

    return 0;
}
