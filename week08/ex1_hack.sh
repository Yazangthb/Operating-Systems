#!/bin/bash
#RUN THE FOLLOWING IN TERMIANL BEFORE RUNNING THIS FILE
#gcc ex1.c -o ex1
#./ex1 &

# run with sudo please

pid=$(cat /tmp/ex1.pid)
if [ -z "$pid" ]; then
    echo "Can't find PID"
    exit 1
fi

# Calculate the address range for the heap memory
maps_file="/proc/$pid/maps"
start=$(grep -m 1 'heap' "$maps_file" | awk '{print $1}' | cut -d'-' -f1)
end=$(grep -m 1 'heap' "$maps_file" | awk '{print $1}' | cut -d'-' -f2)

# Convert the start address to decimal
start_decimal=$((16#$start))

# Read the memory using xxd with the calculated byte offset
password_hex=$(sudo xxd -s $start_decimal -l $((0x$end - 0x$start)) /proc/$pid/mem | grep -A 8 "pass:" | head -9)

if [ -z "$password_hex" ]; then
    echo "Password not found in the heap memory."
    exit 1
fi

# Extract the password in hexadecimal and its memory address
password_hex=$(echo "$password_hex" | grep "pass:" | cut -d' ' -f2-)

# Convert the hexadecimal string to ASCII
password=$(echo -e "$password_hex" | xxd -r -p)

echo "password: $password"
echo "address: 0x$start"

# Send SIGKILL to ex1.c
kill -SIGKILL $pid
