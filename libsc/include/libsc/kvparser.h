#pragma once

#define MAX_OPTION 32
#define MAX_KEY_LENGTH 64
#define MAX_OPT_LENGTH 256

typedef struct kv_map kv_map;

struct kv_map 
{
    int count;
    char keys[MAX_OPTION][MAX_KEY_LENGTH];
    char vars[MAX_OPTION][MAX_OPT_LENGTH];
};


typedef void(*kv_action)(const char* val);

int kv_load(kv_map* kv, const char* path);
int kv_save(kv_map* kv, const char* path);
int kv_add(kv_map* kv, const char* key, const char* val);
const char* kv_get(kv_map* kv, const char* key);

int kv_for_each(kv_map* kv, const char* name, kv_action action);
