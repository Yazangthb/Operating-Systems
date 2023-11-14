#!/bin/bash

mkdir temp1
gcc monitor.c -o monitor
gnome-terminal -- sh -c "./monitor ."
sleep 2
gcc ex1.c -o ex1
./ex1 .
bash ex1_test.sh
