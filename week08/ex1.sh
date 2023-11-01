#!/bin/bash

gcc ex1.c -o ex1
./ex1 &
sleep 3
# Check if the process ID file exists
if [ -f /tmp/ex1.pid ]; then
    # Read the process ID from the file
    pid=$(cat /tmp/ex1.pid)
    echo "ex1.sh: The PID is: $pid"
    # Loop to read the virtual memory and find the password
    while true; do
        password_address=$(sudo gdb -p $pid -batch -ex "info proc mappings" | grep 'pass:' | awk '{print $1}')
        echo "I found the address!"
       
            password=$(sudo gdb -p $pid -batch -ex "x/s $password_address" | awk '{print $5}')
            echo "hello"
            echo $password
            echo "Password: $password (Address: $password_address)"
            echo "hello"
            sudo kill -9 $pid
            break
       
    done

else
    echo "ex1.c process is not running oW'r /tmp/ex1.pid does not exist."
fi

