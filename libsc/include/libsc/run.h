#pragma once

typedef struct sc_run_opts sc_run_opts;

struct sc_run_opts
{
    char* name;
    char* program;

    char* mount_bind_opt;
    char* con_name_opt;
};

int sc_run(sc_run_opts* opt);
