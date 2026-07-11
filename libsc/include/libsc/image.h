#pragma once
#include "storage.h"

int create_image(path_storage* st);
int image_write_metadata(const char* path, const char* name, const char* rootfs);
