gcc mmu.c -o mmu
pager_pid=$(cat /tmp/pager.pid)
sleep 4
sudo ./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 "$pager_pid"
rm mmu
