#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 63
#define MAX_PATH_LENGTH 2048
#define MAX_DATA_SIZE 1024
#define MAX_FILE_DATA 1024
// Struct to represent a file
struct File {
    unsigned int id;
    char name[MAX_NAME_LENGTH + 1];
    unsigned int size;
    char data[MAX_DATA_SIZE + 1];
    struct Directory *directory;
};

// Struct to represent a directory
struct Directory {
    char name[MAX_NAME_LENGTH + 1];
    struct File *files[MAX_DATA_SIZE];
    struct Directory *subdirs[MAX_DATA_SIZE];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH_LENGTH];
};

// Function to overwrite the content of a file
void overwriteToFile(struct File *file, const char *str) {
    if (file) {
        strncpy(file->data, str, MAX_DATA_SIZE);
        file->data[MAX_DATA_SIZE] = '\0';
        file->size = strlen(file->data);
    }
}

// Function to append content to a file
void appendToFile(struct File *file, const char *str) {

    if (file) {
        strncat(file->data, str, MAX_DATA_SIZE - file->size);
        file->data[MAX_DATA_SIZE] = '\0';
        file->size = strlen(file->data);
    }
}

// Function to print the path of a file
void printFilePath(struct File *file) {

    if (file) {
        printf("%s\n", file->name);
        printf("Path: %s/%s\n", file->directory->path,file->name);
    }
}

// Function to add a file to a directory
void addFile(struct File *file, struct Directory *dir) {
    if (file && dir) {
        dir->files[dir->nf++] = file;
        file->directory = dir;
    }
}
void show_file(struct File *file)
{
    printf("%s",file->name);
}
// // Displays the content of tghe Directory dir
void show_dir(struct Directory *dir)
{
printf("\nDIRECTORY\n");
printf(" path: %s\n", dir->path);
printf(" files:\n");
printf(" [ ");
for (int i = 0; i < dir->nf; i++)
{
show_file((dir->files[i]));
}
printf("]\n");
printf(" directories:\n");
printf(" { ");
for (int i = 0; i < dir->nd; i++)
{
show_dir((dir->subdirs[i]));
}
printf("}\n");
}

// Adds the subdirectory dir1 to the directory dir2
void add_dir(struct Directory *dir1, struct Directory *dir2)
{
if (dir1 && dir2)
{
dir2->subdirs[dir2->nd] = dir1;
dir2->nd++;
char temp_path[MAX_PATH_LENGTH];
if (strcmp(dir2->path, "/"))
{
strcpy(temp_path, dir2->path);
strcat(temp_path, "/");
strcat(temp_path, dir1->name);
strcpy(dir1->path, temp_path);
}
else
{
strcpy(temp_path, "/");
strcat(temp_path, dir1->name);
strcpy(dir1->path, temp_path);
}
}
}
int main() {
    // Create the root directory (/)
    struct Directory root;
    strcpy(root.name, "/");
    root.nf = 0;
    root.nd = 0;
    strcpy(root.path, "/");

    // Create the bin directory and add a file bash
    struct Directory bin;
    strcpy(bin.name, "bin");
    bin.nf = 0;
    bin.nd = 0;

    add_dir(&bin,&root);
    struct Directory home;
    strcpy(home.name, "Home");
    home.nf = 0;
    home.nd = 0;
    add_dir(&home,&root);

    struct File bash;
    strcpy(bash.name,"bash");
    bash.id=1;
    bash.size=20;
    overwriteToFile(&bash, "Bourne Again Shell!!");
    bash.directory=&bin;
    addFile(&bash, &bin);

    struct File ex3_1;
    strcpy(ex3_1.name,"ex3_1.c");
    ex3_1.id=2;
ex3_1.size=44;
    strcpy(ex3_1.data,"int printf(const char * format, ...);");
    ex3_1.directory=&home;
    addFile(&ex3_1, &home);

    struct File ex3_2;
    strcpy(ex3_2.name,"ex3_2.c");
    ex3_2.id=3;
    ex3_2.size=30;
    strcpy(ex3_2.data,"//This is a comment in C language");
    ex3_2.directory=&home;
    addFile(&ex3_2, &home);
    // // Append content to ex3 1.c
    // struct File *ex31 = &root.files[1];
    appendToFile(&bash,"int main(){printf(\"Hello World!\");}");

    // Print the paths of all files
     
    printf("Paths of system files:\n");
    printFilePath(&bash);
    printFilePath(&ex3_1);
    printFilePath(&ex3_2);

    return 0;
}
