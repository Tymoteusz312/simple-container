#pragma once

typedef struct runtime_request runtime_request;

struct runtime_request
{
    enum 
    {
        RT_RUN
    } type;

    union
    {
        struct 
        {
            const char* program;
        } run;
    };
};
