#!/bin/bash
gcc ex3.c -o ex3
./ex3 3 &
sleep 10
pstree
./ex3 5 &
sleep 10
pstree
rm ex3
