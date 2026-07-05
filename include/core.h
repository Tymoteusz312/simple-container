#pragma once

#include "types.h"

#define MAX_POSITIONALS 64
#define MAX_TOKENS 64
#define MAX_DATA 256

struct context
{
    int argc;
    char** argv;
    int start;

    command* cmd;

    token* tokens;
    int tokens_size;

    option* options;
    int options_size;

    char* positionals[MAX_POSITIONALS];
    int positional_count;
    int current_positional;

    unsigned char* data[MAX_DATA];
     
} ;

struct option
{
    const char* const long_str;
    const char* const short_str;
    int takes_value; 
    char* description;
} ;

typedef int(*cmd_handler)(context*);
typedef int(*options_parser)(context*);

struct command
{
    const char* name;
    cmd_handler handler;
    options_parser parser;
    option* options;
    int option_count;
    int positional_count;
} ;

context make_ctx(int argc, char** argv);
