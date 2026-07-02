#pragma once

#include "core.h"

int find_cmd(const char* const cmd);
int parse_opt(context* ctx);
int execute_cmd(context* ctx);
