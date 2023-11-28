#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>


int main() {
    // Define the path to the input device and the output file
    const char *devicePath = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    const char *outputFile = "ex1.txt";

    // Open the input device file for reading
    int fileDescriptor = open(devicePath, O_RDONLY);

    // Open the output file for writing
    FILE *outputFilePtr = fopen(outputFile, "w");

    // Structure to store input events
    struct input_event event;

    // Array to store the state of each key (pressed or not pressed) and this array is indexed by the code of each letter (not by ascii)
    int letter_code[256] = {0};

    // print available shortcuts
    printf("Available shortcuts to print messages:\n");
    printf("Shortcut: P+E -> \"I passed the Exam!\"\n");
    printf("Shortcut: C+A+P -> \"Get some cappuccino!\"\n");
    printf("Custom message:O+S -> \"OS assignment\"");
    printf("To terminate the code enter: X+E\n");

    // Infinite loop to continuously monitor input events
    while (1) {
        // Read an input event from the device
        ssize_t bytesRead = read(fileDescriptor, &event, sizeof(event));

        // Check if the event is a key event and if it is a key press, release, or repeat
        if (event.type == EV_KEY && (event.value == 0 || event.value == 1 || event.value == 2)) {
            // Determine the case of the key event
            const char *eventType = (event.value == 0) ? "RELEASED" :
                                    (event.value == 1) ? "PRESSED" :
                                    (event.value == 2) ? "REPEATED" : "UNKNOWN";
            
            // Update the state of the key in the array
            letter_code[event.code] = (event.value == 1);

            // Check for specific key combinations and print corresponding messages
            if (letter_code[25] && letter_code[18]) { //25 is the code for the letter p and 18 for e
                printf("Shortcut: P+E -> \"I passed the Exam!\"\n");
                fprintf(outputFilePtr, "Shortcut: P+E -> \"I passed the Exam!\"\n");
            } else if (letter_code[46] && letter_code[30] && letter_code[25]) { //46 is the code for the char c, 30 for a, and 25 for P
                printf("Shortcut: C+A+P -> \"Get some cappuccino!\"\n");
                fprintf(outputFilePtr, "Shortcut: C+A+P -> \"Get some cappuccino!\"\n");
            } else if (letter_code[24] && letter_code[31]) {// 24 for o and 31 for s
                printf("Custom Message:O+S -> \"OS assignment\"");
                fprintf(outputFilePtr, "Custom Message:O+S -> \"OS assignment\"");
            }

            // Print information about the key event to the console
            printf(" %s 0x%04X (%d)\n", eventType, event.code, event.code);

            // Write information about the key event to the output file
            fprintf(outputFilePtr, " %s 0x%04X (%d)\n", eventType, event.code, event.code);

            // Check for the termination key combination
            if (letter_code[18] && letter_code[45]) {
                break;
            }
        }
    }

    // Close the output file and the input device file
    fclose(outputFilePtr);
    close(fileDescriptor);

    // Return 0 to indicate successful execution
    return 0;
}

