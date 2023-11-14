bash gen.sh 10 ex1.txt
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt
diff ex1.txt ex11.txt
diff ex1.txt ex12.txt
ls -i ex1.txt ex11.txt ex12.txt > output.txt
du -h ex1.txt
mv ex13.txt /tmp/
find . -inum $(stat -c "%i" ex1.txt)
find / -inum $(stat -c "%i" ex1.txt)
stat -c "%h" ex1.txt #1
find ./ex1.txt -inum $(stat -c "%i" ex1.txt) -exec rm {} \;

#    File Content Equality:
 #       The content in ex1.txt, ex11.txt, and ex12.txt is identical due to the creation of hard links with the ln command. Hard links establish multiple directory entries pointing to the same inode on the disk. Modifications to one file reflect in the others since they share the same underlying data. Commands like diff or cat show no distinctions among the files.

#    Inode Numbers:
 #       The identical inode numbers for ex1.txt, ex11.txt, and ex12.txt indicate they are hard links with the same underlying data. Different inode numbers would suggest separate files with distinct data.

  #  Search Paths:
   #     Current Path (.):
    #        Searching in the current path is limited to the current working directory and its subdirectories. It provides a focused and quicker search within a specific area.
     #   Root Path (/):
      #      Searching in the root path covers the entire filesystem starting from the root directory. It is broader and may take longer but is useful when searching across the entire system for files or links.
