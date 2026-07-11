
#include <unistd.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <fcntl.h>

#include <limits.h>
#include <errno.h>

#define MAX_PATH_LEN PATH_MAX

#define BUFFER_SIZE 65536

int mkdirp(const char* path, mode_t mode)
{
    if (path == NULL || *path == '\0')
        return -1;

    if (access(path, F_OK) == 0)
        return 0;

    char tmp[512];
    size_t len = strlen(path);

    if (len >= sizeof(tmp))
    {
        
        fprintf(stderr, "Path too long\n");
        return -1;
    }

    strlcpy(tmp, path, sizeof(tmp));

    if (tmp[len-1] == '/')
        tmp[len-1] = '\0';

    for (char* p = tmp + 1; *p; ++p)
    {
        if (*p == '/')
        {
            *p = '\0';

            if (mkdir(tmp, mode) != 0 && errno != EEXIST)
            {
                perror("mkdir");
                return -1;
            }

            *p = '/';
        }
    }

    if (mkdir(tmp, mode) != 0 && errno != EEXIST)
    {
        perror("mkdir");
        return -1;
    }

    return 0;
}

int copy_file(const char* src_path, const char* dest_path, mode_t mode)
{
    int sfd = open(src_path, O_RDONLY);
    if (sfd == -1)
    {
        fprintf(stderr, "Copying %s: %s\n", src_path, strerror(errno));
        return 0;
    }

    int fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, mode & 0777);
    if (fd == -1)
    {
        perror("destination open");
        return 1;
    }

    // printf("copy: %s -> %s\n", src_path, dest_path);
    // fflush(stdout);

    char buf[BUFFER_SIZE];

    ssize_t bytes;

    while ((bytes = read(sfd, buf, sizeof(buf))) > 0)
    {
        ssize_t written = 0;

        while (written < bytes)
        {
            ssize_t ret = write(fd, buf + written, bytes - written);

            if (ret == -1)
            {
                perror("write");
                close(sfd);
                close(fd);
                return 1;
            }
            written += ret;
        }
    }

    if (bytes == -1)
    {
        perror("read");
        return 1;
    }


    close(sfd);
    close(fd);

    chmod(dest_path, mode & 0777);
    
    return 0;
}

int copy_symlink(const char* src_path, const char* dest_path)
{
    char target[MAX_PATH_LEN];

    ssize_t len = readlink(src_path, target, sizeof(target)-1);

    if (len == -1)
    {
        perror("readlink");
        return 1;
    }

    target[len] = '\0';

    if (symlink(target, dest_path) == -1)
    {
        perror("symlink");
        return 1;
    }

    return 0;
}

int copy_dir(const char* src_path, const char* dest_path)
{
    DIR* directory = opendir(src_path);
    
    if (!directory)
    {
        perror("opendir");
        return 1;
    }

    if (mkdir(dest_path, 0755) == -1)
    {
        fprintf(stderr, "mkdir %s: %s", dest_path, strerror(errno));
        return 1;
    }

    struct dirent* entry;

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char src[MAX_PATH_LEN];
        snprintf(src, MAX_PATH_LEN, "%s/%s", src_path, entry->d_name);

        char dest[MAX_PATH_LEN];
        snprintf(dest, MAX_PATH_LEN, "%s/%s", dest_path, entry->d_name);
        
        struct stat st;
        lstat(src, &st);

        if (S_ISDIR(st.st_mode))
        {
           // if (copy_dir(src, dest)) return 1;
           copy_dir(src, dest);
        }
        else if (S_ISREG(st.st_mode))
        {
            copy_file(src, dest, st.st_mode);
        }
        else if (S_ISLNK(st.st_mode))
        {
            copy_symlink(src, dest);
        }
    }

    closedir(directory);
    return 0;
}
