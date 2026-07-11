#include "image.h"

#include "storage.h"
#include "fsystem.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

#include <fcntl.h>

int create_image(path_storage* st)
{
    if (mkdir(st->image.base, 0755))
    {
        perror("mkdir");
        return 1;
    }

    copy_dir(st->cur_rootfs, st->image.rootfs);
    return 0 ;
}



int image_write_metadata(const char* path, const char* name, const char* rootfs)
{
    if (path == NULL || name == NULL || rootfs == NULL)
    {
        return 1;
    }
    
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0755);

    if (fd == -1)
    {
        perror("write metadat open");
        return 1;
    }

    char buf[512];

    snprintf(buf, sizeof(buf), "NAME=%s\n", name);
    write(fd, buf, strlen(buf));

    snprintf(buf, sizeof(buf), "ROOTFS=%s\n", rootfs);
    write(fd, buf, strlen(buf));

    snprintf(buf, sizeof(buf), "PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin");
    write(fd, buf, strlen(buf));

    close(fd);

    return 0;
}
