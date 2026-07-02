#include "dispacher.h"
#include "commands_registry.h"

#include <string.h>
#include <stdio.h>

int find_cmd(const char* const cmd)
{
    for (int i = 0; i < CMD_COUNT; ++i)
    {
        if (strcmp(cmd, cmds[i].name) == 0)
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

    cmds[ctx->cmd_idx].parser(ctx);
    return 0;
}

int execute_cmd(context* ctx)
{
    return cmds[ctx->cmd_idx].handler(ctx);
}
