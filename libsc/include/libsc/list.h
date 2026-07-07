#pragma once 

typedef struct list_opts list_opts;

struct list_opts
{
    int show_bases_opt;
};

int sc_list(list_opts* opt);
