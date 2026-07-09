#pragma once

typedef struct sc_run_opts sc_run_opts;

struct sc_run_opts
{
    const char* name;
    const char* program;

};

int sc_run(sc_run_opts* opt);
