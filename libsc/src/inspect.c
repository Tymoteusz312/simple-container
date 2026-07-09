#include "inspect.h"

#include "storage.h"
#include "utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>


int sc_inspect(sc_inspect_opts* opt)
{
    if (opt == NULL || opt->image_name == NULL || opt->parameter == NULL)
    {
        return 1;
    }

    storage_init();

    char path[MAX_PATH_LEN];

    snprintf(path, MAX_PATH_LEN, "%s/%s/%s", storage_image_path(), opt->image_name, "config.cfg");

    if (is_path_exist(path))
    {
        fprintf(stderr, "Image %s doesn't exist", opt->image_name);
    }

    int fd = open(path, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    char buf[512];

    read(fd, buf, 512);

    close(fd);

    puts(buf);

    return 0;
}

