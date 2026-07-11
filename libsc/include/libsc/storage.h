#pragma once

#include <limits.h>

#define MAX_PATH_LEN PATH_MAX

typedef struct
{
    char base[MAX_PATH_LEN];
    char rootfs[MAX_PATH_LEN];
    char config[MAX_PATH_LEN];
} image_paths;

typedef struct
{
    char base[MAX_PATH_LEN];
    char rootfs[MAX_PATH_LEN];
    char upper[MAX_PATH_LEN];
    char lower[MAX_PATH_LEN];
    char merged[MAX_PATH_LEN];
} container_path;

typedef struct path_storage path_storage;

struct path_storage
{
    char home[MAX_PATH_LEN];

    char images[MAX_PATH_LEN];
    char rootfs[MAX_PATH_LEN];
    char cur_rootfs[MAX_PATH_LEN];
    char containers[MAX_PATH_LEN];
    char runtime[MAX_PATH_LEN];

    image_paths image;
    container_path container;
};

static const char* get_home();

int init_path_storage(path_storage* st, const char* image_name, const char* container_name, const char* rootfs_name);

//
// int storage_init();
// int storage_bootstrap();
//
// const char* storage_home_path();
// const char* storage_image_path();
// const char* storage_container_path();
// const char* storage_rootfs_path();
