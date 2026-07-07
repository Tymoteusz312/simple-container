#pragma once

typedef struct sc_inspect_opts sc_inspect_opts;

struct sc_inspect_opts
{
    const char* image_name;
    const char* parameter;
};

int sc_inspect(sc_inspect_opts* opt);
