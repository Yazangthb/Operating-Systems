#!/bin/bash

# Compile the C program
gcc ex4.c -o ex4

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the shell program
    ./ex4

    # Clean up the executable
    rm ex4
else
    echo "Compilation failed."
fi
rm ex4
