#!/bin/bash
#note in case the files don;t work this way open two termianls and run pager.sh then in another terminal mmu.sh
# Compile mmu.c and pager.c
gcc mmu.c -o mmu
gcc pager.c -o pager

# Run the pager in the background
./pager <num_pages> <num_frames> &

# Run the MMU with appropriate arguments
./mmu <num_pages> <reference_string> <pager_pid>

# Clean up
rm mmu pager

