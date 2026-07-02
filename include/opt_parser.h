#pragma once
#include "core.h"

typedef struct
{
    enum
    {
        LONG_OPT,
        SHORT_OPT,
        ARG,
        SIZE
    } type;

    int idx;
    
}token;


int tokenize(context* ctx);
