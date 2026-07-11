#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#include "commands/run.h"

#include "core.h"
#include "opt_parser.h"

#include <stdio.h>
#include <string.h>


#include "libsc/run.h"

#define STACK_SIZE (1024 * 1024)

typedef struct 
{
    const char* name;
    const char* program;

    const char* mount_bind_opt;
    const char* con_name_opt;
} run_opt_t;

option run_options[] = 
{
    {"mount-bind", "b", 1, "Specify what path do you want to bind. Format: --mount-bind <SRC>:<DEST>"},
    {"name", "n", 1, "Specify name for container"},
};

enum run_options_types
{
    MOUNT_BIND_OPT,
    NAME_OPT
};

const command run_command=
{
    "run",
    run_handler,
    run_parser,
    &run_options[0],
    sizeof(run_options)/sizeof(run_options[0]),
    2
};

int run_parser(context* ctx)
{
    token* tokens = ctx->tokens;
    int tokens_size = ctx->tokens_size;

    run_opt_t* opt = (run_opt_t*)(ctx->data);

    opt->name = pos_str(ctx);
    opt->program = pos_str(ctx);

    opt->mount_bind_opt = opt_str(ctx, MOUNT_BIND_OPT, NULL);
    opt->con_name_opt = opt_str(ctx, NAME_OPT, NULL);

    return 0;
}


int run_handler(context* ctx)
{
    run_opt_t* opt = (run_opt_t*)(ctx->data);

    sc_run_opts opts = 
    {
        .name = opt->name,
        .program = opt->program,
        .mount_bind_opt = opt->mount_bind_opt,
        .con_name_opt = opt->con_name_opt
    };

    sc_run(&opts);
    return 0;
}




