#pragma once

#include "types.h"

typedef enum
{
    LONG_OPT_TOKEN,
    SHORT_OPT_TOKEN,
    ARG_TOKEN,
    POSITIONAL_TOKEN
}token_type;

struct token
{
    const char* str;
    token_type type;
};

int lex_args(context* ctx);
const char* get_val(const option opt, token* tokens, int tokens_size);
int get_flag(const option opt, token* tokens, int tokens_size);
int get_positionals(context* ctx, token* output);
int parse_int(const char* str);

int opt_int(const char* val, int def);
const char* opt_str(const char* val, const char* def);

int is_long_opt(const char* const str);
int is_short_opt(const char* const str);
int is_argument(const char* const str);

// char* find_opt(option* opt, const context* ctx, char* def, parse_result* err);


