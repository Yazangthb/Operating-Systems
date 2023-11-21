#!/bin/bash
fallocate -l 50M lofs.img #Create a 50 MB image file named lofs.img using fallocate.
losetup -f lofs.img #Associate the image file with a loop device using losetup.
mkfs -t ext4 lofs.img #Create an ext4 file system on the image file using mkfs.
sudo rm -r ./lofsdisk #Remove the existing lofsdisk directory if it exists
mkdir -p ./lofsdisk #create a new directory
sudo mount -o loop lofs.img ./lofsdisk #Mount the loop device to the lofsdisk directory.
gcc ex1.c -o ex1
#Create two text files (file11 and file2) with specific content in the lofsdisk directory.
echo "Yazan" > ./lofsdisk/file11
echo "Alnakri" > ./lofsdisk/file2

#ldd $1: This command prints the shared libraries required by the specified executable ($1). 
#The awk command processes the output of ldd to extract the paths of the shared libraries
#The sed command is used to replace the trailing comma (,) with a newline character (\n). The reason for this is to format the output
get_libs() {
    ldd $1 | awk 'BEGIN{ORS=" "}$1~/^\//{print $1}$3~/^\//{print $3}' | sed 's/,$/\n/'
}

# Specify the loop file system directory
lofsdisk="./lofsdisk"

# Commands to get shared libraries for
commands=("bash" "cat" "echo" "ls")


mkdir -p ./lofsdisk/bin
mkdir -p ./lofsdisk/lib
mkdir -p ./lofsdisk/lib64

# Loop through the commands and copy their shared libraries to the loop file system
for cmd in "${commands[@]}"; do
    libs=$(get_libs "$(which $cmd)")
    echo "$(which $cmd)"
	cp "$(which $cmd)" ./lofsdisk/bin
    # Copy shared libraries to the loop file system
    for lib in $libs; do
        if [ -f "$lib" ]; then
            cp "$lib" ./lofsdisk/lib
            cp "$lib" ./lofsdisk/lib64
            echo "Copied $lib to $lofsdisk"
        else
            echo "Error: Shared library $lib not found."
        fi
    done
done

#Copy an executable file named ex1 to the loop file system and perform additional file operations.
cp ./ex1 ./lofsdisk/bin/ex1
touch ./lofsdisk/bin/ex1.txt
chmod +w ./lofsdisk/bin/ex1.txt

#Use chroot to change the root directory to lofsdisk and execute a command (bin/ex1 > bin/ex1.txt) within that environment.
sudo chroot ./lofsdisk /bin/bash -c "bin/ex1 > bin/ex1.txt"
rm ex1.txt
touch ex1.txt


#Perform cleanup operations and copy the results to the host system.
cp ./lofsdisk/bin/ex1.txt ./ex1.txt
echo " ">>ex1.txt
echo " ">>ex1.txt
echo " ">>ex1.txt

#Add the results of the ex1 when the root is not changed
./ex1 >> ex1.txt
echo "The difference between the outputs is due to the fact that we changed th root directory for ex1 in the first time and made it lofsdisk using chroot" >>ex1.txt
