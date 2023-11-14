#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <signal.h>
#include <string.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
#define PATH_MAX 4096

char *input[PATH_MAX];

void print_stat_info(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        printf("Stat info for %s:\n", path);
        printf("  Size: %ld\n", st.st_size);
        printf("  Inode: %ld\n", st.st_ino);
        printf("  Mode: %o\n", st.st_mode);
        printf("  UID: %d\n", st.st_uid);
        printf("  GID: %d\n", st.st_gid);
        printf("  Last access time: %ld\n", st.st_atime);
        printf("  Last modification time: %ld\n", st.st_mtime);
        printf("  Last status change time: %ld\n", st.st_ctime);
    } else {
        perror("stat");
    }
}

void print_all_stat_info(const char *path) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    printf("Printing stat info for all files and folders in %s:\n", path);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char full_path[PATH_MAX];
            snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);
            print_stat_info(full_path);
        }
    }

    closedir(dir);
}

void sigint_handler(int signo) {
    printf("\nSIGINT received. Printing stat info for all entries before termination.\n");
    // Print stat info for all entries before termination
    print_all_stat_info(*input);
    printf("Terminating in 10 seconds");
    sleep(10);
    exit(EXIT_SUCCESS);
}

void handle_inotify_events(int fd) {
    int length, i = 0;
    int wd;
    char buffer[EVENT_BUF_LEN];

    while (1) {
        wd = inotify_add_watch(fd, *input, IN_CREATE | IN_DELETE | IN_ACCESS | IN_MODIFY | IN_OPEN | IN_ATTRIB);

        length = read(fd, buffer, EVENT_BUF_LEN);

        if (length < 0) {
            perror("read");
        }

        print_stat_info(*input);

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];


            if (event->len) {
            
                if (event->mask & IN_CREATE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("New directory %s created.\n", event->name);
                    }
                    else
                    {
                        printf("New file %s created.\n", event->name);
                    }
                }
                else if (event->mask & IN_DELETE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s deleted.\n", event->name);
                    }
                    else
                    {
                        printf("File %s deleted.\n", event->name);
                    }
                }
                else if (event->mask & IN_ACCESS)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s accessed.\n", event->name);
                    }
                    else
                    {
                        printf("File %s accessed.\n", event->name);
                    }
                }
                else if (event->mask & IN_MODIFY)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s modified.\n", event->name);
                    }
                    else
                    {
                        printf("File %s modified.\n", event->name);
                    }
                }
                else if (event->mask & IN_OPEN)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s opened.\n", event->name);
                    }
                    else
                    {
                        printf("File %s opened.\n", event->name);
                    }
                }
                else if (event->mask & IN_ATTRIB)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s attrib.\n", event->name);
                    }
                    else
                    {
                        printf("File %s attrib.\n", event->name);
                    }
                }
       
            }

            char full_path[PATH_MAX];
            snprintf(full_path, PATH_MAX, "%s/%s", *input, event->name);
            print_stat_info(full_path);
            i += EVENT_SIZE + event->len;

            i += EVENT_SIZE + event->len;
        }

        i = 0;
    }

    inotify_rm_watch(fd, wd);
}

int main(int argc, char *argv[]) {
    *input = argv[1];
    int fd;

    signal(SIGINT, sigint_handler);

    fd = inotify_init();

    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    handle_inotify_events(fd);

    close(fd);

    return 0;
}

