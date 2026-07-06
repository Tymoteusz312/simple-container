#include "utils.h"

#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

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
