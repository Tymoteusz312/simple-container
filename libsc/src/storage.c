#include "storage.h"

#include "utils.h"

#include <pwd.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct storage storage;

struct storage
{
    char home_path[MAX_PATH_LEN];
    char containers_path[MAX_PATH_LEN];
    char images_path[MAX_PATH_LEN];
    char base_rootfs_path[MAX_PATH_LEN];
};

static storage s_st;
static int initialized = 0;

storage* storage_get()
{
    if (!initialized)
    {
        fprintf(stderr, "Storage is not initialized. Call function storage_init() before using storage");
        abort();
    }
    return &s_st;
}

static const char* get_home()
{
    struct passwd* pw = getpwuid(getuid());

    if (pw == NULL)
    {
        perror("getuid");
        return NULL;
    }

    return pw->pw_dir;
}

int storage_init()
{
    const char* home_path = get_home();

    if (home_path == NULL)
    {
        fprintf(stderr, "Getting home directory failed");
        abort();
    }

    strlcpy(s_st.home_path, get_home(), MAX_PATH_LEN);
    snprintf(s_st.containers_path, MAX_PATH_LEN, "%s/.local/share/sc/containers", s_st.home_path);
    snprintf(s_st.images_path, MAX_PATH_LEN, "%s/.local/share/sc/images", s_st.home_path);
    snprintf(s_st.base_rootfs_path, MAX_PATH_LEN, "%s/.local/share/sc/rootfs", s_st.home_path);
    
    initialized = 1;

    return 0;
}

int storage_bootstrap()
{
    if (!initialized)
    {
        fprintf(stderr,
       "Storage is not initialized. Call function storage_init() before using bootstrap function");
        abort();
    }

    mkdirp(storage_container_path(), 0755); 
    mkdirp(storage_image_path(), 0755); 
    mkdirp(storage_rootfs_path(), 0755); 

    return 0;
}

const char* storage_home_path()
{
    return storage_get()->home_path;
}

const char* storage_image_path()
{
    return storage_get()->images_path;
}

const char* storage_container_path()
{
    return storage_get()->containers_path;
}

const char* storage_rootfs_path()
{
    return storage_get()->base_rootfs_path;
}

