#!/bin/bash
fallocate -l 5000M lofs.img
losetup -f lofs.img
mkfs -t ext4 lofs.img
sudo rm -r ./lofsdisk
mkdir -p ./lofsdisk
sudo mount -o loop lofs.img ./lofsdisk
echo "Yazan" > ./lofsdisk/file11
echo "Alnakri" > ./lofsdisk/file2


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
    echo "hi:"
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
cp ./ex1 ./lofsdisk/bin/ex1
touch ./lofsdisk/bin/ex1.txt
chmod +w ./lofsdisk/bin/ex1.txt

sudo chroot ./lofsdisk /bin/bash -c "bin/ex1 > bin/ex1.txt"
rm ex1.txt
touch ex1.txt
cp ./lofsdisk/bin/ex1.txt ./ex1.txt
./ex1 >> ex1.txt

