#include "commands/run.h"

#include <stdio.h>

const option run_options[] = 
{
    {"test", LONG_OPT, "Long test option"},
    {"t", SHORT_OPT, "Short test option"},
    {"i", FLAG, "Random test flag"}
};

const command run_command=
{
    "run",
    run_handler,
    run_parser,
    &run_options[0],
    sizeof(run_options)/sizeof(run_options[0])
};

int run_parser(context* ctx)
{
    puts("Parsing run arguments");
    return 0;
}

int run_handler(context* ctx)
{
    puts("Run test");
    return 0;
}
