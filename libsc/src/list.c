#include "list.h"

#include "storage.h"

#include <stdio.h>
#include <string.h>

// Cala wszystko co korzysta z tego powinno byc w  innym pliku
#include <sys/stat.h>
#include <dirent.h>

int sc_list(list_opts* opt)
{
    if (opt == NULL)
    {
        return 1;
    }

    storage_init();

    const char* path;

    if (opt->show_bases_opt)
        path = storage_rootfs_path();
    else
        path = storage_image_path();

    DIR* directory = opendir(path);

    if (!directory)
    {
        perror("opendir");
        return 1;
    }

    struct dirent* entry;

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        puts(entry->d_name);
    }

    return 0;
}
