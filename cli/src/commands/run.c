#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#include "commands/run.h"

#include "core.h"
#include "opt_parser.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/stat.h>

#include "libsc/run.h"

#define STACK_SIZE (1024 * 1024)

typedef struct 
{
    const char* name;
    const char* program;
} run_opt_t;

option run_options[] = 
{
};

enum run_options_types
{
    null,
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

    return 0;
}


int child_fn(void* arg)
{
    run_opt_t* opt = arg;

    const char* const name = "container";

    sethostname(name, strlen(name));

    mkdir("/proc", 0555);

    mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);
    mount("proc", "/proc", "proc", 0, NULL);


    execvp(opt->name, NULL);

    perror("execvp");
    _exit(1);
}


int run_handler(context* ctx)
{
    run_opt_t* opt = (run_opt_t*)(ctx->data);

    sc_run_opts opts = 
    {
        .name = opt->name,
        .program = opt->program
    };
    sc_run(&opts);
    return 0;
}




