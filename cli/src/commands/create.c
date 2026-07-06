#include "commands/create.h"


#include "request_type.h"
#define _GNU_SOURCE
#define _DEFAULT_SOURCE

#include "core.h"
#include "opt_parser.h"

#include <stdio.h>

#include "libsc/image.h"

typedef struct 
{
    const char* name;
    const char* rootfs_path;
} create_opt_t;

option create_options[] = 
{
    {"args", "a", 1, "Arguments to pass to a new process"},
};

enum create_options_types
{
    ARGS_OPT = 0,
};

const command create_command=
{
    "create",
    create_handler,
    create_parser,
    &create_options[0],
    sizeof(create_options)/sizeof(create_options[0]),
    1
};

int create_parser(context* ctx)
{
    puts("Parsowanie create!");

    token* tokens = ctx->tokens;
    int tokens_size = ctx->tokens_size;

    create_opt_t* opt = (create_opt_t*)(ctx->data);
    
    opt->name = pos_str(ctx);
    opt->rootfs_path = pos_str(ctx);

    puts("Parsowanie create zakonczone");
    return 0;
}




int create_handler(context* ctx)
{
    puts("Run Handler");

    create_opt_t* opt = (create_opt_t*)(ctx->data);

    sc_create_image(opt->name, opt->rootfs_path);

    puts("Run handler ends");

    return 0;
}




