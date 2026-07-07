
#include "commands/inspect.h"

#include "core.h"

#define _GNU_SOURCE
#define _DEFAULT_SOURCE

#include "opt_parser.h"

#include "libsc/inspect.h"

typedef struct 
{
    const char* image_name;

    const char* parameter_opt;

} inspect_opt_t;

option inspect_options[] = 
{
    {"parameter", "p", 1, "Choose which parameter do you want to see"},
};

enum inspect_options_types
{
    PARAMETER_OPT = 0
};

const command inspect_command=
{
    "inspect",
    inspect_handler,
    inspect_parser,
    &inspect_options[0],
    sizeof(inspect_options)/sizeof(inspect_options[0]),
    0
};

int inspect_parser(context* ctx)
{
    token* tokens = ctx->tokens;
    int tokens_size = ctx->tokens_size;

    inspect_opt_t* opt = (inspect_opt_t*)(ctx->data);
    
    opt->image_name = pos_str(ctx);
    opt->parameter_opt = opt_str(ctx, PARAMETER_OPT, "\0");

    return 0;
}




int inspect_handler(context* ctx)
{

    inspect_opt_t* opt = (inspect_opt_t*)(ctx->data);

    sc_inspect_opts opts = 
    {
        .image_name = opt->image_name,
        .parameter = opt->parameter_opt
    };

    sc_inspect(&opts);

    return 0;
}




