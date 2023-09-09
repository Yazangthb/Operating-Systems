mkdir folder_root
date
sleep 3
touch ~/folder_root/root.txt
date
sleep 3
mkdir folder_home
date
sleep 3
touch ~/folder_home/home.txt
ls -lt -r / > ~/folder_root/root.txt
ls -lt -r ~ > ~/folder_home/home.txt
cat ~/folder_root/root.txt
cat ~/folder_home/home.txt
