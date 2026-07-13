#include "storage.h"

#include "fsystem.h"

#include <pwd.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char* get_home()
{
    int uid = getuid();
    struct passwd* pw;

    if (uid == 0)
    {
        const char* username = getenv("SUDO_USER");
        pw = getpwnam(username);
    }
    else
    {
        pw = getpwuid(uid);
    }


    if (pw == NULL)
    {
        perror("getuid");
        return NULL;
    }

    return pw->pw_dir;
}

int init_path_storage(path_storage* st, const char* image_name, const char* container_name, const char* rootfs_name)
{

    const char* home_path = get_home();

    if (home_path == NULL)
    {
        fprintf(stderr, "Getting home directory failed");
        abort();
    }

    snprintf(st->home, MAX_PATH_LEN, "%s/.local/share/sc", home_path);
    snprintf(st->runtime, MAX_PATH_LEN, "%s/runtime", home_path);
    snprintf(st->containers, MAX_PATH_LEN, "%s/containers", st->home);
    snprintf(st->images, MAX_PATH_LEN, "%s/images", st->home);
    snprintf(st->rootfs, MAX_PATH_LEN, "%s/rootfs", st->home);

    if (image_name != NULL)
    {
        snprintf(st->image.base, MAX_PATH_LEN, "%s/%s", st->images, image_name);
        snprintf(st->image.rootfs, MAX_PATH_LEN, "%s/rootfs", st->image.base);
        snprintf(st->image.config, MAX_PATH_LEN, "%s/config.cfg", st->image.base);
    }

    if (container_name != NULL)
    {
        snprintf(st->container.base, MAX_PATH_LEN, "%s/%s", st->containers, container_name);
        snprintf(st->container.rootfs, MAX_PATH_LEN, "%s/rootfs", st->container.base);

        snprintf(st->container.upper, MAX_PATH_LEN, "%s/upper", st->container.base);
        snprintf(st->container.lower, MAX_PATH_LEN, "%s/lower", st->container.base);
        snprintf(st->container.merged, MAX_PATH_LEN, "%s/megred", st->container.base);
        snprintf(st->container.work, MAX_PATH_LEN, "%s/workdir", st->container.base);
    }

    if (rootfs_name != NULL)
    {
        snprintf(st->cur_rootfs, MAX_PATH_LEN, "%s/%s", st->rootfs, rootfs_name);
    }

    return 0;
}

int storage_bootstrap(path_storage* st)
{
    if (st == NULL)
    {
        fprintf(stderr,
       "Storage is not initialized. Call function storage_init() before using bootstrap function");
        abort();
    }

    mkdirp(st->containers, 0755); 
    mkdirp(st->images, 0755); 
    mkdirp(st->rootfs, 0755); 

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// typedef struct storage storage;
//
// struct storage
// {
//     char home_path[MAX_PATH_LEN];
//     char containers_path[MAX_PATH_LEN];
//     char images_path[MAX_PATH_LEN];
//     char base_rootfs_path[MAX_PATH_LEN];
// };
//
// static storage s_st;
// static int initialized = 0;
//
// storage* storage_get()
// {
//     if (!initialized)
//     {
//         fprintf(stderr, "Storage is not initialized. Call function storage_init() before using storage");
//         abort();
//     }
//     return &s_st;
// }
//
// int storage_init()
// {
//     const char* home_path = get_home();
//
//     if (home_path == NULL)
//     {
//         fprintf(stderr, "Getting home directory failed");
//         abort();
//     }
//
//     strlcpy(s_st.home_path, get_home(), MAX_PATH_LEN);
//     snprintf(s_st.containers_path, MAX_PATH_LEN, "%s/.local/share/sc/containers", s_st.home_path);
//     snprintf(s_st.images_path, MAX_PATH_LEN, "%s/.local/share/sc/images", s_st.home_path);
//     snprintf(s_st.base_rootfs_path, MAX_PATH_LEN, "%s/.local/share/sc/rootfs", s_st.home_path);
//     
//     initialized = 1;
//
//     return 0;
// }
//
//
// const char* storage_home_path()
// {
//     return storage_get()->home_path;
// }
//
// const char* storage_image_path()
// {
//     return storage_get()->images_path;
// }
//
// const char* storage_container_path()
// {
//     return storage_get()->containers_path;
// }
//
// const char* storage_rootfs_path()
// {
//     return storage_get()->base_rootfs_path;
// }
//
