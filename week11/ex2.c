#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024
#define DISK_SIZE (128 * 1024)
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

int find_free_inode(FILE *disk) {
    // Find a free inode
    struct inode temp;
    for (int i = 0; i < NUM_FILES; i++) {
        fseek(disk, BLOCK_SIZE + i * sizeof(struct inode), SEEK_SET);
        fread(&temp, sizeof(struct inode), 1, disk);
        if (temp.used == 0) {
            return i;
        }
    }
    return -1; // No free inode found
}

void create(char name[16], int size, FILE *disk) {
    // Check if file already exists
    struct inode temp;
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    for (int i = 0; i < NUM_FILES; i++) {
        fread(&temp, sizeof(struct inode), 1, disk);
        if (temp.used && strcmp(temp.name, name) == 0) {
            printf("Error: File '%s' already exists.\n", name);
            return;
        }
    }

    // Find a free inode
    int freeInodeIndex = find_free_inode(disk);
    if (freeInodeIndex == -1) {
        printf("Error: No free inode available.\n");
        return;
    }

    // Create new file
    struct inode newFile;
    memset(&newFile, 0, sizeof(struct inode));
    strncpy(newFile.name, name, 16);
    newFile.size = size;
    newFile.used = 1;

    // Update inode on disk
    fseek(disk, BLOCK_SIZE + freeInodeIndex * sizeof(struct inode), SEEK_SET);
    fwrite(&newFile, sizeof(struct inode), 1, disk);

    // Update free block list
    char freeBlockList[BLOCK_SIZE];
    fseek(disk, 0, SEEK_SET);
    fread(freeBlockList, BLOCK_SIZE, 1, disk);

    for (int i = 0; i < size; i++) {
        int freeBlock = -1;
        for (int j = 0; j < DISK_SIZE / BLOCK_SIZE; j++) {
            if (freeBlockList[j] == 0) {
                freeBlock = j;
                freeBlockList[j] = 1;
                break;
            }
        }

        if (freeBlock == -1) {
            printf("Error: Not enough free blocks available.\n");
            return;
        }

        // Update block pointer in inode
        newFile.blockPointers[i] = freeBlock;

        // Mark block as used
        fseek(disk, freeBlock * BLOCK_SIZE, SEEK_SET);
        fwrite("1", 1, 1, disk);
    }

    // Update free block list on disk
    fseek(disk, 0, SEEK_SET);
    fwrite(freeBlockList, BLOCK_SIZE, 1, disk);

    printf("File '%s' created successfully.\n", name);
}

void delete(char name[16], FILE *disk) {
    struct inode temp;
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    int found = 0;
    for (int i = 0; i < NUM_FILES; i++) {
        fread(&temp, sizeof(struct inode), 1, disk);
        if (temp.used && strcmp(temp.name, name) == 0) {
            // Mark the corresponding blocks as free
            for (int j = 0; j < temp.size; j++) {
                fseek(disk, temp.blockPointers[j] * BLOCK_SIZE, SEEK_SET);
                fwrite("0", 1, 1, disk); // Mark block as free
            }

            // Mark the inode as free
            temp.used = 0;
            fseek(disk, BLOCK_SIZE + i * sizeof(struct inode), SEEK_SET);
            fwrite(&temp, sizeof(struct inode), 1, disk);

            printf("File '%s' deleted successfully.\n", name);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Error: File '%s' not found.\n", name);
    }
}

void read(char name[16], int blockNum, char buf[1024], FILE *disk) {
    struct inode temp;
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    int found = 0;
    for (int i = 0; i < NUM_FILES; i++) {
        fread(&temp, sizeof(struct inode), 1, disk);
        if (temp.used && strcmp(temp.name, name) == 0) {
            if (blockNum >= 0 && blockNum < temp.size) {
                fseek(disk, temp.blockPointers[blockNum] * BLOCK_SIZE, SEEK_SET);
                fread(buf, 1, BLOCK_SIZE, disk);
                printf("Read from file '%s', Block %d: %s\n", name, blockNum, buf);
                found = 1;
            } else {
                printf("Error: Invalid block number for file '%s'.\n", name);
            }
            break;
        }
    }

    if (!found) {
        printf("Error: File '%s' not found.\n", name);
    }
}

void write(char name[16], int blockNum, char buf[1024], FILE *disk) {
    struct inode temp;
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    int found = 0;
    for (int i = 0; i < NUM_FILES; i++) {
        fread(&temp, sizeof(struct inode), 1, disk);
        if (temp.used && strcmp(temp.name, name) == 0) {
            if (blockNum >= 0 && blockNum < temp.size) {
                fseek(disk, temp.blockPointers[blockNum] * BLOCK_SIZE, SEEK_SET);
                fwrite(buf, 1, BLOCK_SIZE, disk);
                printf("Write to file '%s', Block %d: %s\n", name, blockNum, buf);
                found = 1;
            } else {
                printf("Error: Invalid block number for file '%s'.\n", name);
            }
            break;
        }
    }

    if (!found) {
        printf("Error: File '%s' not found.\n", name);
    }
}

void ls(FILE *disk) {
    struct inode temp;
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    printf("File System Contents:\n");
    for (int i = 0; i < NUM_FILES; i++) {
        fread(&temp, sizeof(struct inode), 1, disk);
        if (temp.used) {
            printf("File: %s, Size: %d blocks\n", temp.name, temp.size);
        }
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <disk_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *disk = fopen(argv[1], "r+");
    if (disk == NULL) {
        perror("Error opening disk file");
        exit(EXIT_FAILURE);
    }

    initialize_disk(disk);

    // Example: Create a file
    create("example.txt", 3, disk);

    // TODO: Process input file and perform operations

    fclose(disk);

    return 0;
}
