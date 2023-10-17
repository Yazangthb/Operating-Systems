#!/bin/bash

# Compile worker.c
gcc worker.c -o worker

# Compile scheduler.c
gcc scheduler.c -o scheduler

# Run the scheduler with the data file as an argument (replace 'data.txt' with your data file)
./scheduler data.txt

# Clean up the compiled worker and scheduler
rm worker scheduler

