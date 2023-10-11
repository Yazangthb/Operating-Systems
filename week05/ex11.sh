#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <num_subscribers>"
    exit 1
fi

num_subscribers=$1

# Compile the publisher and subscriber programs
gcc -o publisher publisher.c
# Start the publisher in the background
./publisher $num_subscribers &
