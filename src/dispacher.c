#include "dispacher.h"
#include "commands_registry.h"
#include "opt_parser.h"
#include "core.h"

#include <string.h>
#include <stdio.h>

int find_cmd(const char* const cmd)
{
    for (int i = 0; i < CMD_COUNT; ++i)
    {
        if (strcmp(cmd, cmds[i]->name) == 0)
            return i;
    }
    return -1;
}

int parse_opt(context* ctx)
{
    if (ctx == NULL)
    {
        perror("ParseOptions funcion: context = NULL");
        return 1;
    }


    token tokens[ctx->argc-ctx->start]; 
    int tokens_size = 0;

    lex_args(ctx, tokens, &tokens_size);



    cmds[ctx->cmd_idx]->parser(tokens, tokens_size, (void*)(ctx->data));
    return 0;
}

int execute_cmd(context* ctx)
{
    return cmds[ctx->cmd_idx]->handler(ctx->data);
}
