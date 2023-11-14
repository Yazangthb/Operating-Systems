#!/bin/bash

# Step 1: Delete ./tmp folder if it exists and create a symbolic link tmp1 for the folder ./tmp
rm -rf ./tmp
ln -s ./tmp tmp1

# Step 2: Run ls -li
echo "Listing before creating ./tmp:"
ls -li

# Step 3: Create the folder ./tmp
mkdir ./tmp

# Step 4: Run ls -li again
echo "Listing after creating ./tmp:"
ls -li

# Step 5: Create a file ex1.txt using gen.sh and add it to ./tmp
./gen.sh > ./tmp/ex1.txt

# Step 6: Check the folder ./tmp1
echo "Listing ./tmp1:"
ls -li tmp1

# Step 7: Create another symbolic link tmp1/tmp2 to the folder ./tmp (symbolic link to itself). Use only absolute paths for the source.
ln -s "$(pwd)/tmp" tmp1/tmp2

# Step 8: Create another file ex1.txt using the same generator gen.sh and add it to the folder ./tmp1/tmp2
./gen.sh > ./tmp1/tmp2/ex1.txt

# Step 9: Check the content of the sub-folders
echo "Listing ./tmp1/tmp2:"
ls -li tmp1/tmp2

# Step 10: Try to access the sub-folders ./tmp1/tmp2/tmp2/tmp2/....
echo "Trying to access sub-folders ./tmp1/tmp2/tmp2/tmp2/...:"
cd tmp1/tmp2/tmp2/tmp2/...
#we are still in the same directory because it points to iteself

# Step 11: Delete the folder ./tmp and check the symbolic links again
rm -rf ./tmp
echo "Listing after deleting ./tmp:"
ls -li

# Step 12: Delete all other symbolic links you created
rm -rf tmp1
echo "Listing after deleting symbolic links:"
ls -li

