#pragma once

#include "types.h"

int run_parser(struct token* tokens, int tokens_size, void* out);
int run_handler(void* opt_spec);
