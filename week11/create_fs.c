#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024
#define DISK_SIZE 128 * 1024
#define NUM_FILES 16
#define INODE_SIZE 56

struct inode {
    char name[16];
    int size;
    int blockPointers[8];
    int used;
};

void initialize_disk(FILE *disk) {
    // Initialize super block
    char freeBlockList[BLOCK_SIZE];
    memset(freeBlockList, 0, BLOCK_SIZE);
    fwrite(freeBlockList, BLOCK_SIZE, 1, disk);

    // Initialize inodes
    struct inode emptyInode;
    memset(&emptyInode, 0, sizeof(struct inode));
    for (int i = 0; i < NUM_FILES; i++) {
        fwrite(&emptyInode, sizeof(struct inode), 1, disk);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <disk_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *disk = fopen(argv[1], "wb+");
    if (disk == NULL) {
        perror("Error opening disk file");
        exit(EXIT_FAILURE);
    }

    initialize_disk(disk);

    fclose(disk);

    printf("File system created successfully: %s\n", argv[1]);

    return 0;
}
