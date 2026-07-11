#pragma once

#define MAX_OPTION 25
#define MAX_KEY_LENGTH 64
#define MAX_OPT_LENGTH 256

typedef struct config config;

struct config 
{
    const char* path;
    char keys[MAX_OPTION][MAX_OPT_LENGTH];
    char vars[MAX_OPTION][MAX_OPT_LENGTH];
    int count;
};


typedef void(*cfg_action)(const char* val);

int config_parse(config* cfg, const char* path);
const char* config_get(config* cfg, const char* key);
int config_for_each(config* cfg, const char* name, cfg_action  action);
