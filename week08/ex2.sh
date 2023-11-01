#!/bin/bash
#note in case the files don;t work this way open two termianls and run pager.sh then in another terminal mmu.sh

gcc mmu.c -o mmu
gcc pager.c -o pager

mkdir -p "/tmp/ex2"

./pager 4 2 &

sleep 2

pager_pid=$(cat /tmp/pager.pid)
xterm -e ./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 "$pager_pid"


rm pager
rm mmu
