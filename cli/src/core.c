#include "core.h"

#include <stdlib.h>

context make_ctx(int argc, char **argv)
{
    return (context)
    {
        argc,
        argv,
        2,
        NULL,
        NULL,
        0,
        NULL,
        0,
        {},
        0,
        0,
        {}
    };
}
