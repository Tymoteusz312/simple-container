#include "opt_parser.h"
#include "core.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int is_long_opt(const char* const str)
{
    if (strlen(str) < 3) return 0;

    if (str[0] == '-' && str[1] == '-')
        return 1;

    return 0;
}

int is_short_opt(const char* const str)
{
    if (strlen(str) < 2) return 0;

    if (str[0] == '-' && str[1] != '-' && str[1] != '\0')
        return 1;

    return 0;
}

int is_argument(const char *const str)  
{
    if (strlen(str) < 1) return 0;

    if (str[0] != '-') return 1;

    return 0;
}

int find_opt(const option* opt, token* tokens, int tokens_size)
{
    if (tokens == NULL)
    {
        perror("Tokens are NULL");
        return -1;
    }

    for (int i = 0; i < tokens_size; ++i)
    {
        if (tokens[i].type == LONG_OPT_TOKEN &&
            strcmp(opt->long_str, tokens[i].str) == 0|| 
            tokens[i].type == SHORT_OPT_TOKEN &&
            strcmp(opt->short_str, tokens[i].str) == 0)
            return i;
    }
    return -1;
}

const char* get_arg(int opt_idx, token* tokens, int tokens_size)
{
    if (tokens == NULL)
    {
        perror("Tokens are NULL");
        return NULL;
    }

    if (opt_idx + 1 >= tokens_size || tokens[opt_idx+1].type != ARG_TOKEN)
    {
        return NULL;
    }

    return tokens[opt_idx+1].str;
}

const char* get_val(const option opt, token* tokens, int tokens_size)
{
    if (!opt.takes_value)
    {
        perror("[DEBUG] get_val can be used only on options with arguments");
        exit(1);
    }

    int idx = find_opt(&opt, tokens, tokens_size);
    
    if (idx < 0)
        return NULL;

    const char* arg = get_arg(idx, tokens, tokens_size);
    if (arg == NULL)
    {
        fprintf(stderr, "Missing argument for option %s\n", tokens[idx].str);
        return NULL;
    }
    return arg;
}

int get_flag(const option opt, token* tokens, int tokens_size)
{
    if (opt.takes_value)
    {
        perror("[DEBUG] get_flag can be used only on flags");
        exit(1);
    }
    
    int idx = find_opt(&opt, tokens, tokens_size);

    if (idx < 0) return 0;
    
    return 1;
}


int parse_int(const char* str)
{
    if (str == NULL)
        return (int)(~0);

    int length = strlen(str);

    int negative;
    if (str[0] == '-') negative = -1;
    else negative = 1;

    int number = 0;
    for (int i = (negative == -1 ? 1 : 0); i < length; ++i)
    {
        number = number * 10 + (str[i] - '0');
    }

    number*=negative;
    return number;
}

int exist_opt(token token, const option* opt_table, int opt_size_tab)
{
    for (int i = 0; i < opt_size_tab; ++i)
    {
        if (token.type == LONG_OPT_TOKEN &&
            strcmp(opt_table[i].long_str, token.str) == 0|| 
            token.type == SHORT_OPT_TOKEN &&
            strcmp(opt_table[i].short_str, token.str) == 0)
            return i;
    }

    return -1;

}

int get_positionals(context* ctx, token* output)
{
    int idx = 0;
    for (int i = 0; i < ctx->tokens_size; ++i)
    {
        if (ctx->tokens[i].type == POSITIONAL_TOKEN)
            output[idx++] = ctx->tokens[i];

        printf("Token: %s o typie %d\n", ctx->tokens[i].str, ctx->tokens[i].type);

    }
    return 0;
}

int lex_args(context* ctx)
{
    token* tokens = ctx->tokens;
    int* tokens_size = &ctx->tokens_size;
    const option* opt_table = ctx->options;
    int opt_tab_size = ctx->options_size;


    if (tokens == NULL || tokens_size == NULL || opt_table == NULL)
        return -1;

    char** arr = ctx->argv;
    
    *tokens_size = 0;
    int last_opt_idx = -1;

    for (int i = ctx->start; i < ctx->argc; ++i)
    {
        token token;
        if (is_long_opt(arr[i]))
        {
            token.str = arr[i]+2;
            token.type = LONG_OPT_TOKEN;
        }
        else if (is_short_opt(arr[i]))
        {
            token.str = arr[i]+1;
            token.type = SHORT_OPT_TOKEN;
        }
        else
        {    
            token.str = arr[i];

            if (last_opt_idx < 0) 
            {
                token.type = POSITIONAL_TOKEN;
                ctx->positional_count++;
            }
            else if (opt_table[last_opt_idx].takes_value)
                token.type = ARG_TOKEN;
            else
            {
                token.type = POSITIONAL_TOKEN;
                ctx->positional_count++;
            }
            
            last_opt_idx = -1;
            tokens[(*tokens_size)++] = token;
            continue;
        }

        last_opt_idx = exist_opt(token, opt_table, opt_tab_size);
        if (last_opt_idx < 0)
        {
            fprintf(stderr, "Option %s doesn't exist\n", arr[i]);
            exit(1);
        }
        else
            tokens[(*tokens_size)++] = token;
    }
    return 0;
}

int opt_int(const char* val, int def)
{
    return val ? parse_int(val) : def;
}

const char* opt_str(const char* val, const char* def)
{
    return val ? val : def;
}
