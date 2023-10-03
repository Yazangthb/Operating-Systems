#!/bin/bash

# Compile the C program
gcc ex1.c -o ex1

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the program
    ./ex1

    # Clean up the executable
    rm ex1
else
    echo "Compilation failed."
fi
