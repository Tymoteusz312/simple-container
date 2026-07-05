#include "dispacher.h"
#include "commands_registry.h"
#include "opt_parser.h"
#include "core.h"

#include <string.h>
#include <stdio.h>

int find_cmd(context* ctx)
{
    for (int i = 0; i < CMD_COUNT; ++i)
    {
        if (strcmp(ctx->argv[1], cmds[i]->name) == 0)
        {
            ctx->cmd = cmds[i]; 
            ctx->options = cmds[i]->options;
            ctx->options_size = cmds[i]->option_count;
            return 0;
        }
    }
    return 1;
}

int parse_opt(context* ctx)
{
    if (ctx == NULL)
    {
        perror("ParseOptions funcion: context = NULL");
        return 1;
    }

    if (ctx->cmd == NULL)
    {
        fprintf(stderr, "[ DEBUG: parse_opt(context* ctx) ] ctx->cmd = NULL | Call find_cmd(context* ctx) first");
        return 1;
    }


    token tokens[ctx->argc-ctx->start]; 

    ctx->tokens = tokens;
    ctx->tokens_size = 0;
    
    lex_args(ctx);

    if (ctx->positional_count < ctx->cmd->positional_count)
    {
        fprintf(stderr, "Too few arguments for command %s.\nCommand %s requires %d arguments",
                ctx->cmd->name, ctx->cmd->name, ctx->cmd->positional_count);
        return 1;
    }

    if (ctx->cmd->parser(ctx))
        return 1;

    ctx->tokens = NULL;

    return 0;
}

int execute_cmd(context* ctx)
{
    return ctx->cmd->handler(ctx);
}
