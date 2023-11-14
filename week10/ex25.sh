#!/bin/bash

# Step 1: Create an empty file ex5.txt
touch ex5.txt

# Step 2: Remove write permission for everybody
chmod a-w ex5.txt

# Step 3: Grant all permissions to owner and others (not group)
chmod ugo+rwx ex5.txt

# Step 4: Make group permissions equal to user permissions
chmod g=u ex5.txt

# Optional: Display the final permissions
ls -l ex5.txt

