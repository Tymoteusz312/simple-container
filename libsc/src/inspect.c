#include "inspect.h"

#include "config.h"
#include "storage.h"

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>

void bind(const char* path)
{
    char src[MAX_PATH_LEN];
    char dest[MAX_PATH_LEN];

    sscanf(path, "%63[^:]:%63s", src, dest);

    printf("Binding %s -> %s\n", src, dest);
}

int sc_inspect(sc_inspect_opts* opt)
{
    if (opt == NULL || opt->image_name == NULL || opt->parameter == NULL)
    {
        return 1;
    }

    path_storage st;
    init_path_storage(&st, opt->image_name, NULL, NULL);

    int fd = open(st.image.config, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    config cfg;
    if (config_parse(&cfg, st.image.config))
    {
        fprintf(stderr, "Error while parsing config file\n");
        return -1;
    }

    config_for_each(&cfg, "BIND", bind);


    return 0;
}




