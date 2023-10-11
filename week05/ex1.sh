#!/bin/bash

gcc publisher.c -o publisher.out
gcc subscriber.c -o subscriber.out

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <n (number of subscribers)>"
    exit 1
fi

n=$1

if [ "$n" -lt 1 ] || [ "$n" -gt 3 ]; then
    echo "Number of subscribers must be between 1 and 3."
    exit 1
fi

# Start subscribers
for ((i = 1; i <= n; i++)); do
    xterm -e ./subscriber.out $i &
done

# Start publisher
./publisher.out $n
