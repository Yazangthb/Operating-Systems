#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

struct PageTableEntry {
    bool isValid;
    int frameNumber;
    bool isDirty;
    int references;
};

struct TLBEntry {
    int page;
    int frame;
};

int numberOfPages;
int tlbSize;

void PrintPageTable(struct PageTableEntry* pageTable) {
    printf("Page Table:\n");
    for (int i = 0; i < numberOfPages; i++) {
        printf("Page %d ---> Valid=%d, Frame=%d, Dirty=%d, References=%d\n", i, pageTable[i].isValid, pageTable[i].frameNumber, pageTable[i].isDirty, pageTable[i].references);
    }
}

void PrintTLB(struct TLBEntry* tlb) {
    printf("TLB:\n");
    for (int i = 0; i < tlbSize; i++) {
        printf("Entry %d ---> Page=%d, Frame=%d\n", i, tlb[i].page, tlb[i].frame);
    }
}

void HandleSIGCONT(int signo) {
    printf("Received SIGCONT signal. Resuming...\n");
}

int main(int argc, char *argv[]) {
    numberOfPages = atoi(argv[1]);
    tlbSize = numberOfPages * 0.2; // 20% of the page table size
    int pagerPid = atoi(argv[argc - 1]);

    printf("Pager PID: %d\n", pagerPid);
    printf("Process PID: %d\n", getpid());
    sleep(1);

    struct TLBEntry tlb[tlbSize];
    for (int i = 0; i < tlbSize; i++) {
        tlb[i].page = -1;
        tlb[i].frame = -1;
    }

    for (int i = 2; i < argc - 1; i++) {
        int table = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (table == -1) {
            perror("Failed to open the page table");
            exit(EXIT_FAILURE);
        }

        int pageTableSize = sizeof(struct PageTableEntry) * numberOfPages;
        struct PageTableEntry* pageTable = (struct PageTableEntry*)mmap(NULL, pageTableSize, PROT_READ | PROT_WRITE, MAP_SHARED, table, 0);

        if (i == 2) {
            printf("----------------------\n");
            printf("Initialized page table\n");
            PrintPageTable(pageTable);
        }

        char requestType = argv[i][0];
        int requestedPageNumber = argv[i][1] - '0';
        int requestedFrameNumber = -1; // Initialize as -1

        // Check TLB first
        for (int j = 0; j < tlbSize; j++) {
            if (tlb[j].page == requestedPageNumber) {
                requestedFrameNumber = tlb[j].frame;
                break;
            }
        }

        if (requestedFrameNumber == -1) {
            // TLB miss, check the page table
            if (pageTable[requestedPageNumber].isValid == 0) {
                printf("Not a valid page --> Page fault\n");
                pageTable[requestedPageNumber].references = getpid();
                kill(pagerPid, SIGUSR1);

                struct sigaction sa;
                sa.sa_handler = HandleSIGCONT;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = 0;
                sigaction(SIGCONT, &sa, NULL);
                pause();

                requestedFrameNumber = pageTable[requestedPageNumber].frameNumber;
            } else {
                printf("Valid page\n");
                requestedFrameNumber = pageTable[requestedPageNumber].frameNumber;
            }

            // Update TLB with the new mapping (replace the oldest entry)
            for (int j = 0; j < tlbSize - 1; j++) {
                tlb[j] = tlb[j + 1];
            }
            tlb[tlbSize - 1].page = requestedPageNumber;
            tlb[tlbSize - 1].frame = requestedFrameNumber;
        }

        PrintPageTable(pageTable);
        PrintTLB(tlb);
    }

    kill(pagerPid, SIGUSR1);
    sleep(10);
}

