#include "utils.h"

#include <sys/stat.h>

#include <unistd.h>

int is_path_exist(const char* path)
{
    return access(path, F_OK) == 0;
}


