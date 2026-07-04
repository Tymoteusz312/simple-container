#include "commands/run.h"

#include "core.h"
#include "opt_parser.h"

#include <stdio.h>

typedef struct 
{
    int test_opt;
    const char* image_opt;
    int iflag;

} run_opt_t;

const option run_options[] = 
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

int run_parser(token* tokens, int tokens_size, void* out)
{
    puts("Parsowanie run!");
    
    run_opt_t* opt = out;

    opt->test_opt = opt_int(get_val(run_options[0], tokens, tokens_size), 10);
    opt->image_opt = opt_str(get_val(run_options[1], tokens, tokens_size), "alpine");
    opt->iflag = get_flag(run_options[2], tokens, tokens_size);

    puts("Parsowanie run zakonczone");
    return 0;
}

int run_handler(void* opt_spec)
{
    puts("Run Handler");

    run_opt_t* opt = opt_spec;

    printf("Opcja test ustawiona na wartosc: %d\n", opt->test_opt);
    printf("Opcja image ustawiona na wartosc: %s\n", opt->image_opt);
    printf("Opcja interactive ustawiona na wartosc %d\n", opt->iflag);

    puts("Run handler ends");

    return 0;
}




