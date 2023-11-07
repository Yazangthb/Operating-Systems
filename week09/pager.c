#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

struct PageTableEntry {
    bool isValid;
    int frameNumber;
    bool isDirty;
    int referenced;
};

int P, F;

void PrintPageTable(struct PageTableEntry *pageTable) {
    printf("Page Table\n");
    for (int i = 0; i < P; i++) {
        printf("Page %d ---> Valid=%d, Frame=%d, Dirty=%d, Referenced=%d\n", i, pageTable[i].isValid, pageTable[i].frameNumber, pageTable[i].isDirty, pageTable[i].referenced);
    }
}

// Random page replacement
int random(struct PageTableEntry* page_table) {
    return rand() % F;
}

// NFU page replacement
int nfu(struct PageTableEntry* page_table) {
    int min_references = page_table[0].referenced;
    int page_to_evict = 0;

    for (int i = 1; i < P; i++) {
        if (page_table[i].referenced < min_references) {
            min_references = page_table[i].referenced;
            page_to_evict = i;
        }
    }

    return page_table[page_to_evict].frameNumber;
}

// Aging page replacement
int aging(struct PageTableEntry* page_table) {
    int min_aging_counter = page_table[0].referenced;
    int page_to_evict = 0;

    for (int i = 1; i < P; i++) {
        if (page_table[i].referenced < min_aging_counter) {
            min_aging_counter = page_table[i].referenced;
            page_to_evict = i;
        }
    }

    for (int i = 0; i < P; i++) {
        page_table[i].referenced >>= 1;
    }

    return page_table[page_to_evict].frameNumber;
}

int main(int argc, char *argv[]) {
    P = atoi(argv[1]);
    F = atoi(argv[2]);

    FILE *file = fopen("/tmp/pager.pid", "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    } else {
        pid_t PID = getpid();
        fprintf(file, "%d\n", PID);
        fclose(file);
    }

    char RAM[F][8];
    char disk[P][8];

    size_t pageTableSize = sizeof(struct PageTableEntry) * P;
    sleep(1);

    int table = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (table == -1) {
        perror("Failed to open pagetable");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(table, pageTableSize) == -1) {
        perror("ftruncate failed");
        close(table);
        exit(EXIT_FAILURE);
    }

    struct PageTableEntry *pageTable = (struct PageTableEntry *)mmap(NULL, pageTableSize, PROT_READ | PROT_WRITE, MAP_SHARED, table, 0);

    if (pageTable == MAP_FAILED) {
        perror("mmap failed");
        close(table);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < P; i++) {
        pageTable[i].isValid = false;
        pageTable[i].frameNumber = -1;
        pageTable[i].isDirty = false;
        pageTable[i].referenced = 0;
    }

    printf("----------------------\n");
    PrintPageTable(pageTable);
    printf("----------------------\n");
    printf("Initialized RAM\n");
    printf("RAM array\n");

    for (int i = 0; i < F; i++) {
        strcpy(RAM[i], "empty");
        printf("Frame %d --->\n", i);
    }

    printf("----------------------\n");
    printf("Initialized disk\n");
    printf("Disk array\n");

    char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    for (int i = 0; i < P; i++) {
        char randomMessage[8];

        for (int j = 0; j < 7; j++) {
            randomMessage[j] = characters[rand() % (sizeof(characters) - 1)];
        }

        randomMessage[7] = '\0';
        strcpy(disk[i], randomMessage);
        printf("Page %d ---> %s\n", i, disk[i]);
    }

    int diskAccesses = 0;
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    int Signal;

    while (sigwait(&sigset, &Signal) == 0) {
        int foundNonZeroReference = -1;
        printf("----------------------\n");

        for (int i = 0; i < P; i++) {
            if (pageTable[i].referenced != 0) {
                printf("A disk access request from MMU Process (pid=%d)\n", pageTable[i].referenced);
                printf("Page %d is referenced\n", i);
                foundNonZeroReference = pageTable[i].referenced;
                bool ok = false;

                for (int j = 0; j < F; j++) {
                    if (strcmp(RAM[j], "empty") == 0) {
                        printf("We can allocate it to free frame %d\n", j);
                        printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, j);
                        pageTable[i].frameNumber = j;
                        strcpy(RAM[j], disk[i]);
                        ok = true;
                        pageTable[i].isValid = 1;
                        pageTable[i].isDirty = 0;
                        pageTable[i].referenced = 0;
                        diskAccesses++;
                        break;
                    }
                }

                if (!ok) {
                    int randomFrame = rand() % (F - 1);
                    printf("We do not have free frames in RAM\n");
                    printf("Choose a random victim page %d\n", randomFrame);
                    printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, randomFrame);
                    printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, randomFrame);
                    strcpy(RAM[randomFrame], disk[i]);

                    for (int j = 0; j < P; j++) {
                        if (pageTable[j].frameNumber == randomFrame) {
                            pageTable[j].isValid = 0;
                            pageTable[j].frameNumber = -1;
                        }
                    }

                    pageTable[i].frameNumber = randomFrame;
                    pageTable[i].isValid = 1;
                    pageTable[i].isDirty = 0;
                    pageTable[i].referenced = 0;
                    diskAccesses++;
                }
            }
        }

        if (foundNonZeroReference == -1) {
            printf("%d disk accesses in total\n", diskAccesses);
            printf("Pager is terminated\n");
            exit(0);
        } else {
            printf("RAM array\n");

            for (int i = 0; i < F; i++) {
                printf("Frame %d ---> %s\n", i, RAM[i]);
            }

            printf("Disk accesses: %d so far\n", diskAccesses);
            printf("Resume MMU process\n");
            printf("%d\n", foundNonZeroReference);
            kill(foundNonZeroReference, SIGCONT);
        }
    }
}

