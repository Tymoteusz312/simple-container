#pragma once

#include <sys/stat.h>

int mkdirp(const char* path, mode_t mode);

int copy_file(const char* src_path, const char* dest_path);
int copy_symlink(const char* src_path, const char* dest_path);
int copy_dir(const char* src_path, const char* dest_path);
