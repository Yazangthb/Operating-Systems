#!/bin/bash

# Compile the agent and controller programs
gcc agent.c -o agent
gcc controller.c -o controller

# Run the agent program in the background
./agent &
sleep 1
# Run the controller program
./controller


# Remove the agent and controller executables (optional)
rm agent controller

