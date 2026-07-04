#include "commands/run.h"

#include "core.h"
#include "opt_parser.h"

#include <stdio.h>

typedef struct 
{
    const char* name;
    int count;

    int test_opt;
    const char* image_opt;
    int iflag;
    
    
} run_opt_t;

option run_options[] = 
{
    {"test", "t", 1, "Test option"},
    {"image", "m", 1, "Image option"},
    {"interactive", "i", 0, "Test flag"}
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
    puts("Parsowanie run!");

    token* tokens = ctx->tokens;
    int tokens_size = ctx->tokens_size;

    run_opt_t* opt = (run_opt_t*)(ctx->data);

    opt->test_opt = opt_int(get_val(run_options[0], tokens, tokens_size), 10);
    opt->image_opt = opt_str(get_val(run_options[1], tokens, tokens_size), "alpine");
    opt->iflag = get_flag(run_options[2], tokens, tokens_size);

    token positionals[ctx->positional_count];

    get_pos(ctx, positionals);
    
    opt->name = positionals[0].str;
    opt->count = parse_int(positionals[1].str);

    puts("Parsowanie run zakonczone");
    return 0;
}

int run_handler(context* ctx)
{
    puts("Run Handler");

    run_opt_t* opt = (run_opt_t*)(ctx->data);

    printf("Opcja test ustawiona na wartosc: %d\n", opt->test_opt);
    printf("Opcja image ustawiona na wartosc: %s\n", opt->image_opt);
    printf("Opcja interactive ustawiona na wartosc %d\n", opt->iflag);
    printf("Argument name jest ustawiony na wartosc %s\n", opt->name);
    printf("Argument count jest ustawionny na wartosc %d\n", opt->count);

    puts("Run handler ends");

    return 0;
}




