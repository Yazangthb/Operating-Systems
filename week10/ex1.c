#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#define PATH_MAX 4096

char path[PATH_MAX];
// Function to find all hard links to a given source file
void find_all_hlinks(const char *source)
{
    struct stat file_stat;
    if (stat(source, &file_stat) == -1)
    {
        perror("lstat1");
        exit(EXIT_FAILURE);
    }

    // Print inode, path, and content of the source file
    printf("Source File: %s\n", source);
    printf("Inode: %ld\n", (long)file_stat.st_ino);

    // Open the watched directory
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        struct stat entry_stat;
        if (stat(entry->d_name, &entry_stat) == -1)
        {
        continue;
            perror("lstat2");
            exit(EXIT_FAILURE);
        }

        // Check if the entry is a hard link to the source file
        if (entry_stat.st_ino == file_stat.st_ino && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char resolved_path[PATH_MAX];
            realpath(entry->d_name, resolved_path);
            printf("Hard Link: Inode: %ld, Path: %s\n", (long)entry_stat.st_ino, resolved_path);
        }
    }

    closedir(dir);
}

// Function to unlink all duplicates of a hard link except the last one
void unlink_all(const char *source)
{
    struct stat file_stat;
    if (stat(source, &file_stat) == -1)
    {
        perror("lstat3");
        exit(EXIT_FAILURE);
    }

    // Open the watched directory
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    char last_link[PATH_MAX] = "";
    while ((entry = readdir(dir)) != NULL)
    {
        struct stat entry_stat;
        if (stat(entry->d_name, &entry_stat) == -1)
        {
        continue;
            perror("lstat4");
            exit(EXIT_FAILURE);
        }

        // Check if the entry is a hard link to the source file
        if (entry_stat.st_ino == file_stat.st_ino && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if (last_link[0] != '\0')
            {
                // Unlink the duplicate hard link
                if (unlink(entry->d_name) == -1)
                {
                    perror("unlink");
                    exit(EXIT_FAILURE);
                }
            }
            strcpy(last_link, entry->d_name);
        }
    }

    printf("Kept Hard Link: %s\n", last_link);

    closedir(dir);
}

// Function to create a symbolic link to a source file
void create_sym_link(const char *source, const char *link)
{
    if (symlink(source, link) == -1)
    {
        perror("symlink");
        exit(EXIT_FAILURE);
    }
}

// Signal handler for SIGINT (Ctrl-c)
void sigint_handler(int signum)
{
    // Print stat info of all entries before termination
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        struct stat entry_stat;
        if (stat(entry->d_name, &entry_stat) == -1)
        {
            perror("lstat5");
            exit(EXIT_FAILURE);
        } 

        printf("Stat Info: %s - Inode: %ld\n", entry->d_name, (long)entry_stat.st_ino);
    }

    closedir(dir);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <watched_directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    snprintf(path, PATH_MAX, "%s", argv[1]);

    signal(SIGINT, sigint_handler);
    char full[PATH_MAX];
    snprintf(full, PATH_MAX, "%s/myfile1.txt", argv[1]);

    printf("%s\n", full);
    // Creating file myfile1.txt with content "Hello world."
    FILE *file1 = fopen(full, "w");
    if (file1 == NULL)	
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file1, "Hello world.\n");
    fclose(file1);
    full[PATH_MAX];
    snprintf(full, PATH_MAX, "%s/myfile1.txt", argv[1]);
    char second[PATH_MAX];
    snprintf(second, PATH_MAX, "%s/myfile11.txt", argv[1]);
    // Creating two hard links to myfile1.txt
    link(full, second);
    full[PATH_MAX];
    snprintf(full, PATH_MAX, "%s/myfile1.txt", argv[1]);
    second[PATH_MAX];
    snprintf(second, PATH_MAX, "%s/myfile12.txt", argv[1]);
    link(full, second);

    // Finding all hard links to myfile1.txt
    find_all_hlinks(full);

    // Moving myfile1.txt to another folder
    if (rename("/tmp/myfile1.txt",full) == -1)
    {
        perror("rename");
        exit(EXIT_FAILURE);
    }
    char third[PATH_MAX];
    snprintf(third, PATH_MAX, "%s/myfile11.txt", argv[1]);
    // Modifying myfile11.txt
    FILE *file11 = fopen(third, "a");
    if (file11 == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file11, "Modification.");
    fclose(file11);
    third[PATH_MAX];
    snprintf(third, PATH_MAX, "%s/myfile13.txt", argv[1]);
    // Creating symbolic link myfile13.txt
    create_sym_link("/tmp/myfile1.txt", third);

    // Modifying /tmp/myfile1.txt
    FILE *tmp_file = fopen("/tmp/myfile1.txt", "a");
    if (tmp_file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(tmp_file, "Modification in /tmp/myfile1.txt.");
    fclose(tmp_file);
    third[PATH_MAX];
    snprintf(third, PATH_MAX, "%s/myfile11.txt", argv[1]);
    // Removing duplicates of hard links to myfile11.txt
    unlink_all(third);

    return 0;
}
