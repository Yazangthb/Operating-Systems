#!/bin/bash

# Compile the C program
gcc allocator.c -o allocator

# Check if the compilation was successful
if [ $? -eq 0 ]; then
  echo "Compilation successful."

  # Run the allocator program with the input file "queries.txt"
  ./allocator

  # Clean up the compiled executable
  rm allocator
else
  echo "Compilation failed."
fi

