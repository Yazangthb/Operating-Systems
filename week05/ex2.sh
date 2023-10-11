#!/bin/bash

# Check if the user provided the number of threads as an argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <number_of_threads>"
    exit 1
fi

# Compile the C program with pthread support
gcc -pthread ex2.c -o ex2

if [ $? -eq 0 ]; then
    # Run the program with the specified number of threads
    ./ex2 "$1"
else
    echo "Compilation failed."
fi
