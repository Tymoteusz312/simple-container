#include "container.h"

#include "fsystem.h"
#include "storage.h"

#include <stdio.h>

int create_container(path_storage* st)
{
    if (st == NULL)
        return 1;

    mkdirp(st->container.base, 0755);

    mkdir(st->container.upper, 0755);
    mkdir(st->container.merged, 0755);
    mkdir(st->container.work, 0755);
    
    return 0;
}
