#include "commands/list.h"

#include "core.h"

#define _GNU_SOURCE
#define _DEFAULT_SOURCE

#include "opt_parser.h"

#include "libsc/list.h"

typedef struct 
{
    int show_bases_opt;
} list_opt_t;

option list_options[] = 
{
    {"rootfs", "r", 0, "Show available rootfs bases"},
};

enum list_options_types
{
    ROOTFS_OPT = 0
};

const command list_command=
{
    "list",
    list_handler,
    list_parser,
    &list_options[0],
    sizeof(list_options)/sizeof(list_options[0]),
    0
};

int list_parser(context* ctx)
{
    token* tokens = ctx->tokens;
    int tokens_size = ctx->tokens_size;

    list_opt_t* opt = (list_opt_t*)(ctx->data);
    
    opt->show_bases_opt = opt_flag(ctx, ROOTFS_OPT);

    return 0;
}




int list_handler(context* ctx)
{

    list_opt_t* opt = (list_opt_t*)(ctx->data);

    list_opts set = 
    {
        .show_bases_opt = opt->show_bases_opt
    };

    sc_list(&set);

    return 0;
}




