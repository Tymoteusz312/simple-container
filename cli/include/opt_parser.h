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
const char* get_val(context* ctx, int opt_idx);
int get_positionals(context* ctx, token* output);

int parse_int(const char* str);

int opt_int(context* ctx, int opt_idx, int def);
const char* opt_str(context* ctx, int opt_idx, const char* def);
int opt_flag(context* ctx, int opt_idx);

const char* pos_str(context* ctx);
int pos_int(context* ctx);

int is_long_opt(const char* const str);
int is_short_opt(const char* const str);
int is_argument(const char* const str);

// char* find_opt(option* opt, const context* ctx, char* def, parse_result* err);


