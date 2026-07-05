#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#include <signal.h>
#include "commands/run.h"

#include "core.h"
#include "opt_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/stat.h>

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

    printf("Opcja test ustawiona na wartosc: %s\n", opt->args_opt);
    printf("Argument name jest ustawiony na wartosc %s\n", opt->name);


    char* stack = malloc(STACK_SIZE);
    char* stack_top = stack + STACK_SIZE;

    pid_t pid = clone(child_fn, stack_top, CLONE_NEWUSER | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD, opt);

    if (pid == -1)
    {
        perror("clone");
        return 1;
    }

    int status;
    waitpid(pid, &status, 0);
    printf("Dziecko zakonczylo ze statusem %d\n", status);

    puts("Run handler ends");

    return 0;
}




