#pragma once

#include "types.h"

struct context
{
    int cmd_idx;
    int argc;
    char** argv;
    int start;
    void* data;
} ;

struct option
{
    const char* const long_str;
    const char* const short_str;
    int takes_value; 
    char* description;
} ;

typedef int(*cmd_handler)(void* opt_spec);
typedef int(*options_parser)(token* tokens, int tokens_size, void* out);

struct command
{
    const char* name;
    cmd_handler handler;
    options_parser parser;
    const option* options;
    int option_count;
} ;
