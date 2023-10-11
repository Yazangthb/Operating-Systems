#!/bin/bash

# Compile ex4.c
gcc -o ex4 ex4.c -lpthread

# Define the range [0, n)
n=10000000

# Define values of m
m_values=(1 2 4 10 100)

# Create ex4_res.txt to store timing results
echo "m_value execution_time" > ex4_res.txt

# Measure execution time for different values of m
for m in "${m_values[@]}"; do
    echo -n "$m " >> ex4_res.txt
    time_output=$( { time ./ex4 $n $m 2>&1; } 2>&1 )
    real_time=$(echo "$time_output" | grep "real" | awk '{print $2}')
    echo "$real_time" >> ex4_res.txt
done

# Clean up
rm ex4

echo "Timing results stored in ex4_res.txt"
