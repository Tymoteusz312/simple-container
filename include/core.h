#pragma once

typedef struct
{
    int cmd_idx;
    int argc;
    char** argv;
    int start;
    void* data;
} context;


typedef int(*cmd_handler)(context*);
typedef int(*options_parser)(context*);

typedef struct
{
    const char* name;
    cmd_handler handler;
    options_parser parser;
} command;
