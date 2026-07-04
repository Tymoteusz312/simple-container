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

// char* find_opt(option* opt, const context* ctx, char* def, parse_result* err)
// {
//     char** arr = ctx->argv;
//     for (int i = ctx->start; i < ctx->argc; ++i)
//     {
//         if (is_long_opt(arr[i]) && strcmp(arr[i]+2, opt->long_str) == 0)
//         {
//             if (i+1 < ctx->argc && is_argument(arr[i+1]))
//                 return arr[i+1]; // Puzniej tutaj trzeba bedzie parsowac typ
//             else
//             {
//                 *err = OPT_MISSING_VALUE;
//                 return def;
//             }
//         }
//         else if (is_short_opt(arr[i]) && strcmp(arr[i]+1, opt->short_str) == 0)
//         {
//             if (i+1 < ctx->argc && is_argument(arr[i+1]))
//                 return arr[i+1];
//             else
//             {
//                 *err = OPT_MISSING_VALUE;
//                 return def;
//             }
//         }
//     }
//     return def;
// }

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


int lex_args(const context* ctx, token* tokens, int* tokens_size)
{
    char** arr = ctx->argv;
    for (int i = ctx->start; i < ctx->argc; ++i)
    {
        token_type type;
        int dashes=0;
        if (is_long_opt(arr[i]))
        {
            type = LONG_OPT_TOKEN;
            dashes = 2;
        }
        else if (is_short_opt(arr[i]))
        {
            type = SHORT_OPT_TOKEN;
            dashes = 1;
        }
        else 
        {
            type = ARG_TOKEN;
            dashes = 0;
        }
        tokens[i-ctx->start] = (token){ arr[i] + dashes, type};
        ++(*tokens_size);
    }
    return 1; 
}

int opt_int(const char* val, int def)
{
    return val ? parse_int(val) : def;
}

const char* opt_str(const char* val, const char* def)
{
    return val ? val : def;
}
