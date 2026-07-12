#include "image.h"

#include "storage.h"
#include "fsystem.h"

#include <stdio.h>
#include "kvparser.h"
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

    kv_map kv = {0};

    kv_add(&kv, "NAME", name);
    kv_add(&kv, "ROOTFS", rootfs);
    kv_add(&kv, "PATH", "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin");

    kv_save(&kv, path);
    
    return 0;
}
