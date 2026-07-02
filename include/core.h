#pragma once

typedef struct
{
    int cmd_idx;
    int argc;
    char** argv;
    int start;
    void* data;
} context;

typedef struct
{
    char* str;
    enum
    {
        SHORT_OPT,
        LONG_OPT,
        FLAG,
        POSITIONAL,
        SIZE
    } type;
    char* description;
} option;

typedef int(*cmd_handler)(context*);
typedef int(*options_parser)(context*);

typedef struct
{
    const char* name;
    cmd_handler handler;
    options_parser parser;
    const option* options;
    int option_count;
} command;
