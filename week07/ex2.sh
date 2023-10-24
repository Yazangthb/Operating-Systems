#!/bin/bash

# Compile the C program
gcc ex2.c -o ex2

# Check if the compilation was successful
if [ $? -eq 0 ]; then
  echo "Compilation successful."

  # Run the ex2 program
  ./ex2

  # Clean up the compiled executable
  rm ex2
else
  echo "Compilation failed."
fi

