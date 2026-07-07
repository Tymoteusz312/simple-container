#pragma once

#define MAX_PATH_LEN 256

int storage_init();
int storage_bootstrap();

const char* storage_home_path();
const char* storage_image_path();
const char* storage_container_path();
const char* storage_rootfs_path();
