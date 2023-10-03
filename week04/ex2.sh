#!/bin/bash

# Compile the C program
gcc ex2.c -o ex2 -lm

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the program
    ./ex2

    # Clean up the executable
    rm ex2
else
    echo "Compilation failed."
fi

