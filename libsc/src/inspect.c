#include "inspect.h"

#include "kvparser.h"
#include "storage.h"

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>

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

    kv_map kv;

    kv_load(&kv, st.image.config);

    
    for (int i = 0; i < kv.count; ++i)
    {
        printf("%s = %s\n", kv.keys[i], kv.vars[i]);
    }
    return 0;
}




