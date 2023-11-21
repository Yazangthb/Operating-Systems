#!/bin/bash

# Compile create_fs.c
gcc create_fs.c -o create_fs

# Create the file system
./create_fs disk0

# Run the simulation with ex2.c
gcc ex2.c -o ex2
./ex2 input_file.txt

# Clean up
rm create_fs ex2
