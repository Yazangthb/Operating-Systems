
stat -c " %i" ex1
cp ex1 ex2
stat -c " %i" ex2
echo "Inode numbers of ex1 and ex2:"
stat -c "%i" ex1 ex2
stat -c "%h - %n" /etc/* | grep "^3"
#different inodes because copy creates a new file so if one file changes the other is not effected
