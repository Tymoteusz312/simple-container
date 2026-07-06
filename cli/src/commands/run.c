#include "request_type.h"
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

#include "libsc/runtime.h"

#define STACK_SIZE (1024 * 1024)

typedef struct 
{
    const char* name;
    const char* args_opt;
} run_opt_t;

option run_options[] = 
{
    {"args", "a", 1, "Arguments to pass to a new process"},
};

enum run_options_types
{
    ARGS_OPT = 0,
};

const command run_command=
{
    "run",
    run_handler,
    run_parser,
    &run_options[0],
    sizeof(run_options)/sizeof(run_options[0]),
    1
};

int run_parser(context* ctx)
{
    puts("Parsowanie run!");

    token* tokens = ctx->tokens;
    int tokens_size = ctx->tokens_size;

    run_opt_t* opt = (run_opt_t*)(ctx->data);

    opt->args_opt = opt_str(ctx, ARGS_OPT, "");
    
    opt->name = pos_str(ctx);

    puts("Parsowanie run zakonczone");
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
    puts("Run Handler");

    run_opt_t* opt = (run_opt_t*)(ctx->data);

    runtime_request req = 
    {
        .type = RT_RUN,
        .run = 
        {
            .program = opt->name
        }
    };

    exec_runtime(&req);

    puts("Run handler ends");

    return 0;
}




